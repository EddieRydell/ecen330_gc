#include "tone.h"

#include <stdlib.h>
#include <math.h>

#define BIAS 0x80
#define AMPLITUDE 0x7F

#define ERR_MALLOC_FAILURE (-1)
#define ERR_SAMPLE_RATE_TOO_LOW 1
#define ERR_TONE_BUFFER_NULL 2

static uint8_t* tone_buffer;
static uint32_t sample_rate;

// Initialize the tone driver. Must be called before using.
// May be called again to change sample rate.
// sample_hz: sample rate in Hz to playback tone.
// Return zero if successful, or non-zero otherwise.
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

// Free resources used for tone generation (DAC, DMA hardware, buffers, etc.).
// Return zero if successful, or non-zero otherwise.
int32_t tone_deinit(void) {
    // make sure we don't free a null pointer
    if (tone_buffer == NULL) {
        return ERR_TONE_BUFFER_NULL;
    }
    free(tone_buffer);
    sound_deinit();
    return 0;
}

// Start playing the specified tone.
// tone: one of the enumerated tone types.
// freq: frequency of the tone in Hz.
void tone_start(tone_t tone, uint32_t freq) {
    // make sure tone and frequency are within the correct range
    if (tone >= LAST_T || freq < LOWEST_FREQ || freq > sample_rate / 2) {
        return;
    }
    uint32_t num_samples = sample_rate / freq;
    // generate waveform and store it in tone_buffer based on what kind of wave it should be
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
                float value = ((float)i / ((float)num_samples / 2)) * (2.0f * AMPLITUDE);
                tone_buffer[i] = (uint8_t)fminf(value, 255.0f);  // Ensure the value stays within uint8_t range
            }

            for (uint32_t i = num_samples / 2; i < num_samples; i++) {
                float value = (2.0f * AMPLITUDE) - ((((float)i - (float)num_samples / 2) / ((float)num_samples / 2)) * (2.0f * AMPLITUDE));
                tone_buffer[i] = (uint8_t)fminf(value, 255.0f);  // Ensure the value stays within uint8_t range
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
    tone_test_create_wave(tone_buffer, num_samples);
    sound_cyclic(tone_buffer, num_samples);
}