#include <sys/cdefs.h>
#include <stdlib.h>

#include "esp_log.h"

#include "driver/gptimer.h"
#include "lcd.h"
#include "pin.h"

#define DATA_OUT_PIN 18

#define DEFAULT_TIMER_RESOLUTION 1000000 // 1MHz, 1 tick = 1us
#define FRAMERATE 200
#define TICKS_PER_FRAME (DEFAULT_TIMER_RESOLUTION / FRAMERATE)
#define NUM_LEDS 150

static const char* TAG = "lab06_WS2811";

uint8_t led_data[NUM_LEDS * 3];

bool send_led_data(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {

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
            .on_alarm = send_led_data // register user callback
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));

    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    ESP_ERROR_CHECK(gptimer_start(gptimer));

    while(1) {

    }
}
