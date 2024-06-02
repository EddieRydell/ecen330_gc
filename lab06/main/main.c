#include <sys/cdefs.h>
#include <stdlib.h>
#include <string.h>

// esp headers for RMT, logging, ISRs, and SD card file management
#include "esp_log.h"
#include "driver/gptimer.h"
#include "driver/rmt_tx.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"
#include "freertos/queue.h"
#include "esp_timer.h"
#include "errno.h"
#include "sys/stat.h"
#include "dirent.h"

#include "pin.h"

#define DATA_OUT_PIN 12
#define SD_CS_PIN 22
#define SD_MOSI_PIN 23
#define SD_MISO_PIN 19
#define SD_SCK_PIN 18

#define DEFAULT_TIMER_RESOLUTION 1000000 // 1MHz, 1 tick = 1us
#define FRAMES_PER_SECOND 200
#define TICKS_PER_FRAME (DEFAULT_TIMER_RESOLUTION / FRAMES_PER_SECOND)
#define RMT_RESOLUTION 20000000 // 20MHz
#define RMT_PERIOD 50 // 1/20MHz = 50ns
#define NUM_LEDS 100
#define BITS_PER_LED 24
#define BYTE_SIZE_BITS 8
#define BYTES_PER_LED (BITS_PER_LED / BYTE_SIZE_BITS)

// WS2812 protocol data high and low time ns
#define WS2812_1_HIGH_TIME_NS 800
#define WS2812_1_LOW_TIME_NS 450
#define WS2812_0_HIGH_TIME_NS 400
#define WS2812_0_LOW_TIME_NS 850

#define WS2812_1_HIGH_TIME_TICKS (WS2812_1_HIGH_TIME_NS / RMT_PERIOD)
#define WS2812_1_LOW_TIME_TICKS (WS2812_1_LOW_TIME_NS / RMT_PERIOD)
#define WS2812_0_HIGH_TIME_TICKS (WS2812_0_HIGH_TIME_NS / RMT_PERIOD)
#define WS2812_0_LOW_TIME_TICKS (WS2812_0_LOW_TIME_NS / RMT_PERIOD)

#define RED_CHANNEL 0
#define GREEN_CHANNEL 1
#define BLUE_CHANNEL 2

#define DEFAULT_TASK_STACK_DEPTH 4096

#define DEFAULT_MAX_FILES 5
#define DEFAULT_ALLOCATION_UNIT_SIZE (16 * 1024)

static const char* TAG = "lab06_WS2812";

// Globals here aren't marked as volatile as they're only accessed from one task each
static char led_data[NUM_LEDS * BYTES_PER_LED];

static rmt_channel_handle_t tx_channel;
static rmt_encoder_handle_t encoder;

static TaskHandle_t led_task_handle;

static FILE* sequence_file;

static uint32_t frame_count;
static uint32_t times_task_was_performed;

uint64_t start_time;

void list_directory(const char* path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        ESP_LOGE(TAG, "Failed to open directory: %s", strerror(errno));
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        ESP_LOGI(TAG, "Found file: %s", entry->d_name);
    }
    closedir(dir);
}

// Parse the header of the .fseq file.
// This function advances sequence_file to be at the beginning of the actual LED data
void parse_fseq_header() {
    ESP_LOGI(TAG, "Parsing .fseq file header");
    uint8_t header_data[6];
    size_t bytes_read = fread(header_data, 1, sizeof(header_data), sequence_file);
    if (bytes_read < sizeof(header_data)) {
        if (feof(sequence_file)) {
            ESP_LOGI(TAG, "Reached end of file.");
        }
        else if (ferror(sequence_file)) {
            ESP_LOGE(TAG, "Error reading file.");
        }
    }
    uint16_t data_location = *(header_data + 4);

    // Move the file pointer to the data location
    if (fseek(sequence_file, data_location, SEEK_SET) != 0) {
        ESP_LOGE(TAG, "Error seeking to data location.");
    }
}

// Initialize RMT subsystem
void init_rmt() {
    rmt_tx_channel_config_t tx_channel_config = {
            .clk_src = RMT_CLK_SRC_DEFAULT,
            .gpio_num = DATA_OUT_PIN,
            .mem_block_symbols = 64,           // memory block size, 64 * 4 = 256 Bytes
            .resolution_hz = RMT_RESOLUTION,
            .trans_queue_depth = 4,            // set the number of transactions that can pend in the background
            .flags.invert_out = false,         // do not invert output signal
            .flags.with_dma = false,           // do not need DMA backend
    };

    // Create the RMT TX channel
    esp_err_t ret = rmt_new_tx_channel(&tx_channel_config, &tx_channel);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create TX channel: %s", esp_err_to_name(ret));
        return;
    }

    // Enable the RMT TX channel
    ret = rmt_enable(tx_channel);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to enable TX channel: %s", esp_err_to_name(ret));
        return;
    }

    // Define the RMT bytes encoder configuration
    rmt_bytes_encoder_config_t encoder_config = {
            .bit0 = {
                    .level0 = 1,
                    .duration0 = WS2812_0_HIGH_TIME_TICKS,
                    .level1 = 0,
                    .duration1 = WS2812_0_LOW_TIME_TICKS,
            },
            .bit1 = {
                    .level0 = 1,
                    .duration0 = WS2812_1_HIGH_TIME_TICKS,
                    .level1 = 0,
                    .duration1 = WS2812_1_LOW_TIME_TICKS,
            },
    };

    // Create the RMT bytes encoder
    ret = rmt_new_bytes_encoder(&encoder_config, &encoder);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create bytes encoder: %s", esp_err_to_name(ret));
        return;
    }

    ESP_LOGI(TAG, "RMT and encoder initialized successfully");
}

// Initialize SD card to read files from it
void init_sd_card() {
    ESP_LOGI(TAG, "Initializing SD card");
#define MOUNT_POINT "/sdcard"
#define FILE_NAME "/NOCOMP~1.FSE"
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
            .format_if_mount_failed = false,
            .max_files = DEFAULT_MAX_FILES,
            .allocation_unit_size = DEFAULT_ALLOCATION_UNIT_SIZE // DEFAULT_ALLOCATION_UNIT_SIZE
    };

    esp_err_t ret;
    sdmmc_card_t* card;
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    spi_bus_config_t bus_cfg = {
            .mosi_io_num = SD_MOSI_PIN,
            .miso_io_num = SD_MISO_PIN,
            .sclk_io_num = SD_SCK_PIN,
            .quadwp_io_num = -1,
            .quadhd_io_num = -1,
            .max_transfer_sz = 4000,
    };

    ret = spi_bus_initialize(host.slot, &bus_cfg, (spi_dma_chan_t)SDSPI_DEFAULT_DMA);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize bus: %s", esp_err_to_name(ret));
        return;
    }

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = SD_CS_PIN;
    slot_config.host_id = host.slot;
    ESP_LOGI(TAG, "Success initializing SD card");

    ESP_LOGI(TAG, "Mounting filesystem");
    ret = esp_vfs_fat_sdspi_mount(MOUNT_POINT, &host, &slot_config, &mount_config, &card);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                          "If you want the card to be formatted, set the CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                          "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        return;
    }
    ESP_LOGI(TAG, "Filesystem mounted");
    ESP_LOGI(TAG, "Listing directory contents of %s", MOUNT_POINT);
    list_directory(MOUNT_POINT);

    ESP_LOGI(TAG, "Opening file %s", MOUNT_POINT FILE_NAME);

    sequence_file = fopen(MOUNT_POINT FILE_NAME, "rb");
    if (sequence_file == NULL) {
        ESP_LOGE(TAG, "Failed to open file: %s", strerror(errno));
        // Additional debugging information
        struct stat st;
        if (stat(MOUNT_POINT FILE_NAME, &st) == 0) {
            ESP_LOGI(TAG, "File exists. Permissions: %lo", st.st_mode);
        }
        else {
            ESP_LOGE(TAG, "stat failed: %s", strerror(errno));
        }
        return;
    }
    ESP_LOGI(TAG, "File opened successfully");

    parse_fseq_header();
}

// Use RMT module to transmit LED data out from DATA_OUT_PIN
void IRAM_ATTR send_led_data() {
    rmt_transmit_config_t tx_config = {
            .loop_count = 0, // No looping
    };
    ESP_ERROR_CHECK(rmt_transmit(
            tx_channel,
            encoder,
            led_data,
            sizeof(led_data),
            &tx_config));
}

// task to load the led_data buffer out of the SD card and send it to the LEDs
_Noreturn void load_and_send_led_buffer_task(void* pvParameters) {
    while (1) {
        if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY)) {
            times_task_was_performed++;
            send_led_data();
            size_t bytes_read = fread(led_data, 1, sizeof(led_data), sequence_file);
            if (bytes_read < sizeof(led_data)) {
                if (feof(sequence_file)) {
                    ESP_LOGI(TAG, "Reached end of file.");
                }
                else if (ferror(sequence_file)) {
                    ESP_LOGE(TAG, "Error reading file.");
                }
                uint64_t end_time = esp_timer_get_time();
                ESP_LOGI(TAG, "Frame Count: %lu; Task execution count: %lu; "
                              "Total time: %llu seconds",
                              frame_count, times_task_was_performed, (end_time - start_time) / 1000000);
                ESP_LOGI(TAG, "Restarting...");
                fclose(sequence_file);
                esp_restart();
            }
        }
    }
}

// ISR function to handle sending the data to the LEDs and loading the next buffer of data to send
bool IRAM_ATTR advance_frame(gptimer_handle_t timer, const gptimer_alarm_event_data_t* edata, void* user_ctx) {
    frame_count++;
    BaseType_t higher_priority_task_woken = pdFALSE;
    vTaskNotifyGiveFromISR(led_task_handle, &higher_priority_task_woken);
    return higher_priority_task_woken == pdTRUE;
}

// Main function containing timer setup and execution
_Noreturn void app_main(void) {
    ESP_LOGI(TAG, "Startup - initialize data out pin, frame timer, and RMT subsystem");
    pin_reset(DATA_OUT_PIN);
    pin_output(DATA_OUT_PIN, true);

    init_rmt();
    init_sd_card();

    // Create the LED buffer task
    xTaskCreate(
            load_and_send_led_buffer_task,
            "Frame Task",
            DEFAULT_TASK_STACK_DEPTH,
            NULL,
            5,
            &led_task_handle);

    // preload led_data buffer to read from it at the beginning of the first frame
    xTaskNotifyGive(led_task_handle);

    // init ISR timer
    gptimer_handle_t gptimer = NULL;
    gptimer_config_t timer_config = {
            .clk_src = GPTIMER_CLK_SRC_DEFAULT,
            .direction = GPTIMER_COUNT_UP,
            .resolution_hz = DEFAULT_TIMER_RESOLUTION
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));
    gptimer_alarm_config_t alarm_config = {
            .reload_count = 0,
            .alarm_count = TICKS_PER_FRAME,
            .flags.auto_reload_on_alarm = true,
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));

    gptimer_event_callbacks_t cbs = {
            .on_alarm = advance_frame // register user callback
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));

    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    ESP_ERROR_CHECK(gptimer_start(gptimer));

    ESP_LOGI(TAG, "Beginning main loop");
    start_time = esp_timer_get_time();
    while (1) {}
}
