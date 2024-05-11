#include <stdio.h>
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

volatile bool RUNNING = true;

static const char *TAG = "lab03";

void timer_callback() {

}

void app_main(void)
{
    ESP_LOGI(TAG, "Start up");
    pin_reset(BTN_A);
    pin_reset(BTN_B);
    pin_reset(BTN_START);
    pin_input(BTN_A, true);
    pin_input(BTN_B, true);
    pin_input(BTN_START, true);


    ESP_LOGI(TAG, "Stopwatch update");
    TFT_t dev; // Declare device handle for the display
    lcdInit(&dev); // Initialize LCD and device handle
    watch_init(&dev); // Initialize stopwatch face
    for (;;) { // forever update loop
        watch_update(&dev, timer_ticks);
    }
}