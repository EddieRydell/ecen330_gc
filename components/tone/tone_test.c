#include "tone_test.h"
#include "lcd.h"

#include <malloc.h>

static uint8_t* tone_buffer;
static uint32_t num_samples;

#define WAVE_DRAW_VERTICAL_SCALE 3

void tone_test_draw_wave(TFT_t* dev, uint16_t color) {
    // Make sure tone_buffer has been allocated
    if (!tone_buffer) return;

    // Draw the wave for the first time
    for (int32_t i = 1; i < num_samples; i++) {
        int32_t x0 = LCD_W * ((float)(i - 1) / (float)(num_samples * 2));  // Halve the width
        int32_t y0 = (tone_buffer[i - 1] + LCD_H) / WAVE_DRAW_VERTICAL_SCALE;
        int32_t x1 = LCD_W * ((float)i / (float)(num_samples * 2));  // Halve the width
        int32_t y1 = (tone_buffer[i] + LCD_H) / WAVE_DRAW_VERTICAL_SCALE;

        lcdDrawLine(dev, x0, y0, x1, y1, color);
    }

    // Draw a line between the end of the first wave and the start of the second wave
    int32_t x_last = LCD_W * ((float)(num_samples - 1) / (float)(num_samples * 2));
    int32_t y_last = (tone_buffer[num_samples - 1] + LCD_H) / WAVE_DRAW_VERTICAL_SCALE;
    int32_t x_first_next = LCD_W * ((float)num_samples / (float)(num_samples * 2));
    int32_t y_first_next = (tone_buffer[0] + LCD_H) / WAVE_DRAW_VERTICAL_SCALE;

    lcdDrawLine(dev, x_last, y_last, x_first_next, y_first_next, color);

    // Draw the wave for the second time
    for (int32_t i = 1; i < num_samples; i++) {
        int32_t x0 = LCD_W * ((float)(i - 1 + num_samples) / (float)(num_samples * 2));  // Continue past first wave
        int32_t y0 = (tone_buffer[i - 1] + LCD_H) / WAVE_DRAW_VERTICAL_SCALE;
        int32_t x1 = LCD_W * ((float)(i + num_samples) / (float)(num_samples * 2));  // Continue past first wave
        int32_t y1 = (tone_buffer[i] + LCD_H) / WAVE_DRAW_VERTICAL_SCALE;

        lcdDrawLine(dev, x0, y0, x1, y1, color);
    }
}

void tone_test_create_wave(uint8_t* buffer, uint32_t size) {
    tone_buffer = buffer;
    num_samples = size;
}
