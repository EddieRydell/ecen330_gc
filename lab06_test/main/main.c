#include <sys/cdefs.h>
#include <stdlib.h>

#include "esp_log.h"
#include "driver/gptimer.h"
#include "driver/rmt.h"

#include "pin.h"

#define DATA_OUT_PIN 18

#define DEFAULT_TIMER_RESOLUTION 1000000 // 1MHz, 1 tick = 1us
#define FRAMES_PER_SECOND 200
#define TICKS_PER_FRAME (DEFAULT_TIMER_RESOLUTION / FRAMES_PER_SECOND)
#define RMT_CLOCK_DIVIDER 4 // ABP clock speed is 80MHz. We want a period of 50ns for WS2812 protocol. 50ns * 80MHz = 4
#define NUM_LEDS 150
#define BITS_PER_LED 24
#define BYTE_SIZE_BITS 8
#define BYTES_PER_LED (BITS_PER_LED / BYTE_SIZE_BITS)

// WS2812 protocol data high and low time in ticks for 50ns period
#define WS2812_1_HIGH_TIME 16   // 800ns
#define WS2812_1_LOW_TIME 9     // 450ns
#define WS2812_0_HIGH_TIME 8    // 400ns
#define WS2812_0_LOW_TIME 17    // 850ns

#define GET_BIT_FROM_BYTE(byte,bit) ((byte) & (0x80 >> (bit)))

static const char* TAG = "lab06_WS2812";

volatile uint8_t led_data[NUM_LEDS * BYTES_PER_LED]; // this possibly is able to not be volatile but I need to look into it more

void init_rmt() {
    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(DATA_OUT_PIN, RMT_CHANNEL_0);
    config.clk_div = RMT_CLOCK_DIVIDER;
    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(RMT_CHANNEL_0, 0, 0));
}

void IRAM_ATTR send_led_data() {
    rmt_item32_t items[NUM_LEDS * BITS_PER_LED];  // 24 bits per LED
    for (uint_fast16_t i = 0; i < NUM_LEDS * BYTES_PER_LED; i++) {
        uint8_t byte = led_data[i];
        for (uint_fast8_t bit = 0; bit < BYTE_SIZE_BITS; bit++) {
            if (GET_BIT_FROM_BYTE(byte, bit)) {
                items[i * BYTE_SIZE_BITS + bit].duration0 = WS2812_1_HIGH_TIME;
                items[i * BYTE_SIZE_BITS + bit].level0 = 1;
                items[i * BYTE_SIZE_BITS + bit].duration1 = WS2812_1_LOW_TIME;
                items[i * BYTE_SIZE_BITS + bit].level1 = 0;
            }
            else {
                items[i * BYTE_SIZE_BITS + bit].duration0 = WS2812_0_HIGH_TIME;
                items[i * BYTE_SIZE_BITS + bit].level0 = 1;
                items[i * BYTE_SIZE_BITS + bit].duration1 = WS2812_0_LOW_TIME;
                items[i * BYTE_SIZE_BITS + bit].level1 = 0;
            }
        }
    }
    rmt_write_items(RMT_CHANNEL_0, items, NUM_LEDS * BITS_PER_LED, true);
    rmt_wait_tx_done(RMT_CHANNEL_0, portMAX_DELAY);
}

void IRAM_ATTR load_next_frame_buffer() {

}

bool IRAM_ATTR advance_frame(gptimer_handle_t timer, const gptimer_alarm_event_data_t* edata, void* user_ctx) {
    send_led_data();
    load_next_frame_buffer();
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
            .reload_count = 0, // counter will reload with 0 on alarm event
            .alarm_count = TICKS_PER_FRAME,
            .flags.auto_reload_on_alarm = true, // enable auto-reload
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
