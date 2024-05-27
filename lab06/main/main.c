#include <sys/cdefs.h>
#include <stdlib.h>

#include "esp_log.h"
#include "driver/gptimer.h"
#include "driver/rmt_tx.h"

#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"

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

// WS2812 protocol data high and low time in ticks for 50ns period
#define WS2812_1_HIGH_TIME_NS 800   // 800ns
#define WS2812_1_LOW_TIME_NS 450     // 450ns
#define WS2812_0_HIGH_TIME_NS 400    // 400ns
#define WS2812_0_LOW_TIME_NS 850    // 850ns

#define WS2812_1_HIGH_TIME_TICKS (WS2812_1_HIGH_TIME_NS / RMT_PERIOD)
#define WS2812_1_LOW_TIME_TICKS (WS2812_1_LOW_TIME_NS / RMT_PERIOD)
#define WS2812_0_HIGH_TIME_TICKS (WS2812_0_HIGH_TIME_NS / RMT_PERIOD)
#define WS2812_0_LOW_TIME_TICKS (WS2812_0_LOW_TIME_NS / RMT_PERIOD)

#define RED_CHANNEL 0
#define GREEN_CHANNEL 1
#define BLUE_CHANNEL 2

#define DEFAULT_MAX_FILES 5
#define DEFAULT_ALLOCATION_UNIT_SIZE (16 * 1024)

static const char* TAG = "lab06_WS2812";

// Globals here aren't marked as volatile as they're only accessed from one task each
uint8_t led_data[NUM_LEDS * BYTES_PER_LED];

rmt_channel_handle_t tx_channel;
rmt_encoder_handle_t encoder;

sdmmc_card_t sd_card;
FILE* sequence_file;

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

void init_sd_card() {
    ESP_LOGI(TAG, "Initializing SD card");
#define MOUNT_POINT "/sdcard"
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
#ifdef CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED
            .format_if_mount_failed = true,
#else
            .format_if_mount_failed = false,
#endif // CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED
            .max_files = DEFAULT_MAX_FILES,
            .allocation_unit_size = DEFAULT_ALLOCATION_UNIT_SIZE
    };

    esp_err_t ret;
    sdmmc_card_t *card;
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
        ESP_LOGE(TAG, "Failed to initialize bus.");
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
    sequence_file = fopen(MOUNT_POINT"/sequence.hex", "r");
    if (sequence_file == NULL) {
        ESP_LOGI(TAG, "Null file");
    }
    ESP_LOGI(TAG, "Filesystem mounted");
}

// Use RMT system to transmit LED data out from DATA_OUT_PIN
void IRAM_ATTR send_led_data() {
    //ESP_EARLY_LOGI(TAG, "Transmitting LED data");
    rmt_transmit_config_t tx_config = {
            .loop_count = 0, // No looping
    };
    ESP_ERROR_CHECK(rmt_transmit(tx_channel, encoder, (const void*)led_data, sizeof(led_data), &tx_config));
}

// Function to load data into the led_data buffer for next frame
void IRAM_ATTR load_next_frame_buffer() {
    //ESP_EARLY_LOGI(TAG, "Reading data from SD card");
    char* ret = fgets((char*)led_data, sizeof(led_data), sequence_file);
    if (ret == NULL) {
        //ESP_EARLY_LOGI(TAG, "fgets failed to read data or encountered EOF");
    }
    //ESP_EARLY_LOGI(TAG, "Read data from SD card");
}

// ISR function to handle sending the data to the LEDs and loading the next buffer of data to send
bool IRAM_ATTR advance_frame(gptimer_handle_t timer, const gptimer_alarm_event_data_t* edata, void* user_ctx) {
    //ESP_EARLY_LOGI(TAG, "Start of advance_frame function");
    send_led_data();
    load_next_frame_buffer();
    return true;
}

// Main function containing timer setup and execution
_Noreturn void app_main(void)
{
    ESP_LOGI(TAG, "Startup - initialize data out pin, frame timer, and RMT subsystem");
    pin_reset(DATA_OUT_PIN);
    pin_output(DATA_OUT_PIN, true);

    init_rmt();
    init_sd_card();

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
    while(1) {}
}
