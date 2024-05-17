#include "tone.h"

#include <stdlib.h>
#include <math.h>

#define BIAS 0x80
#define AMPLITUDE 0x7F

static uint8_t* tone_buffer;
static uint32_t sample_rate;

int32_t tone_init(uint32_t sample_hz) {
    if (sample_hz < 2 * LOWEST_FREQ) {
        return ERR_SAMPLE_RATE_TOO_LOW;
    }
    sound_init(sample_hz);
    sample_rate = sample_hz;
    tone_buffer = malloc(sample_hz / LOWEST_FREQ);
    if (tone_buffer == NULL) {
        return ERR_MALLOC_FAILURE;
    }
    return 0;
}

int32_t tone_deinit(void) {
    if (tone_buffer == NULL) {
        return ERR_TONE_BUFFER_NULL;
    }
    free(tone_buffer);
    sound_deinit();
    return 0;
}

void tone_start(tone_t tone, uint32_t freq) {
    if (tone >= LAST_T || freq < LOWEST_FREQ || freq > sample_rate / 2) {
        return;
    }
    uint32_t num_samples = sample_rate / freq;
    switch (tone) {
        case SINE_T:
            for (uint32_t i = 0; i < num_samples; i++) {
                tone_buffer[i] = BIAS + AMPLITUDE * sinf(2 * (float)M_PI * ((float)i / (float)num_samples));
            }
            break;
        case SQUARE_T:
            for (uint32_t i = 0; i < num_samples; i++) {
                tone_buffer[i] = BIAS + ((i > num_samples / 2) ? AMPLITUDE : -AMPLITUDE);
            }
            break;
        case TRIANGLE_T:
            for (uint32_t i = 0; i < num_samples / 2; i++) {
                tone_buffer[i] = (uint8_t)(((float)i / ((float)num_samples / 2)) * (2 * AMPLITUDE));
            }
            for (uint32_t i = num_samples / 2; i < num_samples; i++) {
                tone_buffer[i] = (2 * AMPLITUDE) - (uint8_t)(((float)i / ((float)num_samples / 2)) * (2 * AMPLITUDE));
            }
            break;
        case SAW_T:
            for (uint32_t i = 0; i < num_samples; i++) {
                tone_buffer[i] = (uint8_t)(((float)i / (float)num_samples) * (2 * AMPLITUDE));
            }
            break;
        default:
            break;
    }
    sound_cyclic(tone_buffer, num_samples);
}