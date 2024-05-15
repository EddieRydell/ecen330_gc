#include <stdlib.h>

#include "esp_log.h"

#include "driver/gptimer.h"
#include "lcd.h"
#include "pin.h"
#include "watch.h"

#define BTN_A 32
#define BTN_B 33
#define BTN_MENU 13
#define BTN_OPTION 0
#define BTN_SELECT 27
#define BTN_START 39

#define DEFAULT_TIMER_RESOLUTION 1000000
#define DEFAULT_ALARM_COUNT 10000

volatile bool running = true;
volatile uint32_t timer_ticks;

static const char *TAG = "lab03";

// Logic for the stopwatch.
// Button A starts timer. Button B pauses. Start button resets and pauses the timer.
// IRAM_ATTR ensures that the function will be stored in RAM rather than flash memory (meaning consistent fast execution)
// Arguments are passed but not used in order to match the function signature provided in .on_alarm field of gptimer_event_callbacks_t
bool IRAM_ATTR timer_callback(gptimer_handle_t timer, const gptimer_alarm_event_data_t* edata, void* user_ctx) {
    if (pin_get_level(BTN_A) == 0) {
        running = true;
    }
    if (pin_get_level(BTN_B) == 0) {
        running = false;
    }
    if (pin_get_level(BTN_START) == 0) {
        running = false;
        timer_ticks = 0;
    }
    if (running) {
        timer_ticks++;
    }
    return true;
}

// Main function containing timer setup and execution
void app_main(void)
{
    ESP_LOGI(TAG, "Start up");
    pin_reset(BTN_A);
    pin_reset(BTN_B);
    pin_reset(BTN_START);
    pin_input(BTN_A, true);
    pin_input(BTN_B, true);
    pin_input(BTN_START, true);

    gptimer_handle_t timer = NULL;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = DEFAULT_TIMER_RESOLUTION
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &timer));

    gptimer_event_callbacks_t cbs = {
        .on_alarm = timer_callback // link callback function to my own timer_callback()
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(timer, &cbs, NULL));

    gptimer_alarm_config_t alarm_config = {
        .reload_count = 0, // counter will reload with 0 on alarm event
        .alarm_count = DEFAULT_ALARM_COUNT,
        .flags.auto_reload_on_alarm = true, // enable auto-reload
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(timer, &alarm_config));

    ESP_ERROR_CHECK(gptimer_enable(timer));
    ESP_ERROR_CHECK(gptimer_start(timer));

    ESP_LOGI(TAG, "Stopwatch update");
    TFT_t dev; // Declare device handle for the display
    lcdInit(&dev); // Initialize LCD and device handle
    watch_init(&dev); // Initialize stopwatch face
    for (;;) { // forever update loop
        watch_update(&dev, timer_ticks);
    }
}
