#ifndef TONE_TEST_H
#define TONE_TEST_H

#include <stdint.h>
#include "lcd.h"

void tone_test_create_wave(uint8_t* tone_buffer, uint32_t num_samples);

void tone_test_draw_wave(TFT_t* dev, uint16_t color);

#endif //TONE_TEST_H
