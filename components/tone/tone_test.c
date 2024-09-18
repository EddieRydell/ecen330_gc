#include "tone_test.h"
#include "lcd.h"

#include <malloc.h>

static uint8_t* tone_buffer;
static uint32_t num_samples;

void tone_test_draw_wave(TFT_t* dev, uint16_t color) {
    for (int32_t i = 0; i < num_samples; i++) {
        lcdDrawPixel(dev, LCD_W * ((float)i / (float)num_samples), tone_buffer[i], color);
    }
}

void tone_test_create_wave(uint8_t* buffer, uint32_t size) {
    tone_buffer = buffer;
    num_samples = size;
}
