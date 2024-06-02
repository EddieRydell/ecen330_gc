#include <sys/cdefs.h>
#include <stdlib.h>
#include <string.h>

// esp headers for RMT, logging, ISRs, and SD card file management
#include "esp_log.h"
#include "driver/gptimer.h"
#include "driver/rmt_tx.h"
#include "sdmmc_cmd.h"
#include "esp_timer.h"

#include "pin.h"
#include "sd_card_file_system.h"

#define DATA_OUT_PIN 12

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

#define FILE_NAME "/NOCOMP~1.FSE"

#define DEFAULT_TASK_STACK_DEPTH 4096

static const char* TAG = "lab06_WS2812";

// Globals here aren't marked as volatile as they're only accessed from one task each
static char led_data[NUM_LEDS * BYTES_PER_LED];

static rmt_channel_handle_t tx_channel;
static rmt_encoder_handle_t encoder;

static TaskHandle_t led_task_handle;

static uint32_t frame_count;
static uint32_t times_task_was_performed;

static fseq_sequence_t sequence;

uint64_t start_time;

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
            size_t bytes_read = fread(led_data, 1, sizeof(led_data), sequence.sequence_file);
            if (bytes_read < sizeof(led_data)) {
                if (feof(sequence.sequence_file)) {
                    ESP_LOGI(TAG, "Reached end of file.");
                }
                else if (ferror(sequence.sequence_file)) {
                    ESP_LOGE(TAG, "Error reading file.");
                }
                uint64_t end_time = esp_timer_get_time();
                ESP_LOGI(TAG, "Frame Count: %lu; Task execution count: %lu; "
                              "Total time: %llu seconds",
                              frame_count, times_task_was_performed, (end_time - start_time) / 1000000);
                ESP_LOGI(TAG, "Restarting...");
                fclose(sequence.sequence_file);
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
    sequence = open_and_parse_fseq_file("NOCOMP~1.FSE");

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
