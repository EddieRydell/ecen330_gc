#include <sys/cdefs.h>
#include <stdlib.h>

#include "esp_log.h"
#include "driver/gptimer.h"
#include "driver/rmt_tx.h"
#include "sd_protocol_types.h"

#include "pin.h"

#define DATA_OUT_PIN 18

#define DEFAULT_TIMER_RESOLUTION 1000000 // 1MHz, 1 tick = 1us
#define FRAMES_PER_SECOND 200
#define TICKS_PER_FRAME (DEFAULT_TIMER_RESOLUTION / FRAMES_PER_SECOND)
#define RMT_RESOLUTION 20000000
#define NUM_LEDS 100
#define BITS_PER_LED 24
#define BYTE_SIZE_BITS 8
#define BYTES_PER_LED (BITS_PER_LED / BYTE_SIZE_BITS)

// WS2812 protocol data high and low time in ticks for 50ns period
#define WS2812_1_HIGH_TIME 16   // 800ns
#define WS2812_1_LOW_TIME 9     // 450ns
#define WS2812_0_HIGH_TIME 8    // 400ns
#define WS2812_0_LOW_TIME 17    // 850ns

#define RED_CHANNEL 0
#define GREEN_CHANNEL 1
#define BLUE_CHANNEL 2

static const char* TAG = "lab06_WS2812";

volatile uint8_t led_data[NUM_LEDS * BYTES_PER_LED]; // this possibly is able to not be volatile but I need to look into it more
volatile uint32_t frame_count;

rmt_channel_handle_t tx_channel;
rmt_encoder_handle_t encoder;

// Initialize RMT subsystem
void init_rmt() {
    tx_channel = NULL;
    rmt_tx_channel_config_t tx_channel_config = {
            .gpio_num = DATA_OUT_PIN,
            .clk_src = RMT_CLK_SRC_DEFAULT,
            .resolution_hz = RMT_RESOLUTION,
            .mem_block_symbols = 64, // Memory block size
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_channel_config, &tx_channel));
    ESP_ERROR_CHECK(rmt_enable(tx_channel));

    rmt_bytes_encoder_config_t encoder_config = {
            .bit0 = {
                    .level0 = 1,
                    .duration0 = WS2812_0_HIGH_TIME,
                    .level1 = 0,
                    .duration1 = WS2812_0_LOW_TIME,
            },
            .bit1 = {
                    .level0 = 1,
                    .duration0 = WS2812_1_HIGH_TIME,
                    .level1 = 0,
                    .duration1 = WS2812_1_LOW_TIME,
            },
    };
    ESP_ERROR_CHECK(rmt_new_bytes_encoder(&encoder_config, &encoder));
}

// Use RMT system to transmit LED data out from DATA_OUT_PIN
void IRAM_ATTR send_led_data() {
    rmt_transmit_config_t tx_config = {
            .loop_count = 0, // No looping
    };
    ESP_ERROR_CHECK(rmt_transmit(tx_channel, encoder, (const void*)led_data, sizeof(led_data), &tx_config));
}

// Function to load data into the led_data buffer for next frame
void IRAM_ATTR load_next_frame_buffer() {
    uint32_t count  = frame_count;
    count /= 1000;
    for (uint32_t i = 0; i < NUM_LEDS; i++) {
        led_data[i * BYTES_PER_LED + RED_CHANNEL] = (count + i) % 0xFF;
        led_data[i * BYTES_PER_LED + GREEN_CHANNEL] = (count + i + 0x55) % 0xFF;
        led_data[i * BYTES_PER_LED + BLUE_CHANNEL] = (count + i + 0xAA) % 0xFF;
    }

    /*for (uint32_t i = 0; i < NUM_LEDS; i++) {
        uint8_t value = (count + i) % 0xFF;
        led_data[i * BYTES_PER_LED + RED_CHANNEL] = value;
        led_data[i * BYTES_PER_LED + GREEN_CHANNEL] = value;
        led_data[i * BYTES_PER_LED + BLUE_CHANNEL] = value;
    }*/
}

// Timer callback function to handle sending the data to the LEDs and loading the next buffer of data to send
bool IRAM_ATTR advance_frame(gptimer_handle_t timer, const gptimer_alarm_event_data_t* edata, void* user_ctx) {
    send_led_data();
    load_next_frame_buffer();
    frame_count++;
    return true;
}

// Main function containing timer setup and execution
_Noreturn void app_main(void)
{
    ESP_LOGI(TAG, "Startup - init data out pin and setup frame timer");
    pin_reset(DATA_OUT_PIN);
    pin_output(DATA_OUT_PIN, true);

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
