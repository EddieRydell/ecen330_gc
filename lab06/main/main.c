#include <stdlib.h>

#include "esp_log.h"
#include "driver/gptimer.h"
#include "driver/rmt_tx.h"
#include "sdmmc_cmd.h"
#include "esp_timer.h"

#include "pin.h"
#include "sd_card_file_system.h"
#include "fseq.h"
#include "interface.h"

#define DATA_OUT_PIN 12
#define BTN_A      32
#define BTN_B      33
#define BTN_MENU   13
#define BTN_OPTION  0
#define BTN_SELECT 27
#define BTN_START  39

#define DEBOUNCE_WAIT_TIME 8 // ms

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

#define DEFAULT_TASK_STACK_DEPTH 4096

static const char* TAG = "lab06_WS2812";

// Globals here aren't marked as volatile as they're only accessed from one task each
static uint8_t led_data[NUM_LEDS * BYTES_PER_LED];

static rmt_channel_handle_t tx_channel;
static rmt_encoder_handle_t encoder;

static TaskHandle_t led_task_handle;

static uint32_t frame_count;
static uint32_t times_task_was_performed;

static fseq_sequence_t sequence;

uint64_t start_time;

void print_led_data_as_hex() {
    char hex_string[sizeof(led_data) * 2 + 1];
    hex_string[sizeof(led_data) * 2] = 0;

    for (size_t i = 0; i < sizeof(led_data); ++i) {
        sprintf(hex_string + (i * 2), "%02X", led_data[i]);
    }

    ESP_LOGI(TAG, "%s", hex_string);
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

// Function to reverse the bits of a byte
void reverse_bits(uint8_t* byte) {
    *byte = (*byte & 0xF0) >> 4 | (*byte & 0x0F) << 4;
    *byte = (*byte & 0xCC) >> 2 | (*byte & 0x33) << 2;
    *byte = (*byte & 0xAA) >> 1 | (*byte & 0x55) << 1;
}

// Use RMT module to transmit LED data out from DATA_OUT_PIN
void IRAM_ATTR send_led_data() {
    //print_led_data_as_hex();
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
_Noreturn void IRAM_ATTR load_and_send_led_buffer_task(void* pvParameters) {
    while (1) {
        if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY)) {
            times_task_was_performed++;
            send_led_data();
            if (get_next_led_buffer(led_data, sequence) == 0) {
                uint64_t end_time = esp_timer_get_time();
                ESP_LOGI(TAG, "Frame Count: %lu; Task execution count: %lu; "
                              "Total time: %llu seconds",
                              frame_count, times_task_was_performed, (end_time - start_time) / 1000000);
                ESP_LOGI(TAG, "Restarting...");
                close_sequence(sequence);
                esp_restart();
            }
            // For some reason the lights read all the bytes backwards
            for (uint32_t i = 0; i < sizeof(led_data); i++) {
                reverse_bits(led_data + i);
            }
        }
    }
}

// ISR function to handle sending the data to the LEDs and loading the next buffer of data to send
bool IRAM_ATTR advance_frame(gptimer_handle_t timer, const gptimer_alarm_event_data_t* edata, void* user_ctx) {
    BaseType_t higher_priority_task_woken = pdFALSE;
    vTaskNotifyGiveFromISR(led_task_handle, &higher_priority_task_woken);
    frame_count++;
    return higher_priority_task_woken == pdTRUE;
}

// Main function containing timer setup and execution
_Noreturn void app_main(void) {
    ESP_LOGI(TAG, "Startup - initialize pins");
    pin_reset(DATA_OUT_PIN);
    pin_output(DATA_OUT_PIN, true);
    pin_reset(BTN_A);
    pin_input(BTN_A, true);
    pin_reset(BTN_B);
    pin_input(BTN_B, true);
    pin_reset(BTN_MENU);
    pin_input(BTN_MENU, true);
    pin_reset(BTN_OPTION);
    pin_input(BTN_OPTION, true);
    pin_reset(BTN_SELECT);
    pin_input(BTN_SELECT, true);
    pin_reset(BTN_START);
    pin_input(BTN_START, true);

    init_rmt();

    // These functions must happen in this order.
    // The SD card, LCD, and file system won't work otherwise
    init_display();
    init_sd_card();
    init_filenames();

    // Create the LED buffer task
    xTaskCreate(
            load_and_send_led_buffer_task,
            "Frame Task",
            DEFAULT_TASK_STACK_DEPTH,
            NULL,
            5,
            &led_task_handle);

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

    RESET:
    ESP_LOGI(TAG, "Displaying menu");
    draw_interface();
    while (pin_get_level(BTN_START)) { // while start isn't pressed display the menu
        while (!pin_get_level(BTN_A)) {
            draw_interface();
            vTaskDelay(pdMS_TO_TICKS(DEBOUNCE_WAIT_TIME));
            while (!pin_get_level(BTN_A)); // wait for button release
            vTaskDelay(pdMS_TO_TICKS(DEBOUNCE_WAIT_TIME));
        }
    }

    sequence = open_and_parse_fseq_file(get_current_file());
    // preload led_data buffer to read from it at the beginning of the first frame
    xTaskNotifyGive(led_task_handle);

    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    ESP_ERROR_CHECK(gptimer_start(gptimer));

    ESP_LOGI(TAG, "Beginning main loop");
    start_time = esp_timer_get_time();
    while (1) {
        if (!pin_get_level(BTN_SELECT)) {
            gptimer_stop(gptimer);
            gptimer_disable(gptimer);
            close_sequence(sequence);
            goto RESET;
        }
    }
}
