#include <sys/cdefs.h>
#include <stdlib.h>

#include "esp_log.h"
#include "driver/gptimer.h"
#include "driver/rmt_tx.h"
#include "driver/sdmmc_host.h"

#include "pin.h"

#define DATA_OUT_PIN 12
#define SD_CS_PIN 22
#define SD_MOSI_PIN 23
#define SD_MISO_PIN 19
#define SD_SCLK_PIN 18

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

// Globals here aren't marked as volatile as they're only accessed from inside the advance_frame ISR
uint8_t led_data[NUM_LEDS * BYTES_PER_LED];
uint32_t frame_count;

rmt_channel_handle_t tx_channel;
rmt_encoder_handle_t encoder;

// Initialize RMT subsystem
void init_rmt() {
    // Define the RMT TX channel configuration
    rmt_tx_channel_config_t tx_channel_config = {
            .clk_src = RMT_CLK_SRC_DEFAULT,    // select source clock
            .gpio_num = DATA_OUT_PIN,
            .mem_block_symbols = 64,           // memory block size, 64 * 4 = 256 Bytes
            .resolution_hz = RMT_RESOLUTION,
            .trans_queue_depth = 4,            // set the number of transactions that can pend in the background
            .flags.invert_out = false,         // do not invert output signal
            .flags.with_dma = false,           // do not need DMA backend
    };

    // Create the RMT TX channel
    esp_err_t ret = rmt_new_tx_channel(&tx_channel_config, &tx_channel);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create TX channel: %s", esp_err_to_name(ret));
        return;
    }

    // Enable the RMT TX channel
    ret = rmt_enable(tx_channel);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to enable TX channel: %s", esp_err_to_name(ret));
        return;
    }

    // Define the RMT bytes encoder configuration
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

    // Create the RMT bytes encoder
    ret = rmt_new_bytes_encoder(&encoder_config, &encoder);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create bytes encoder: %s", esp_err_to_name(ret));
        return;
    }

    ESP_LOGI(TAG, "RMT and encoder initialized successfully");
}

void init_sd_card() {
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    host.max_freq_khz = SDMMC_FREQ_HIGHSPEED;

    sdmmc_slot_config_t slot = SDMMC_SLOT_CONFIG_DEFAULT();
    slot.width = 1;


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
    uint32_t count = frame_count;
    count /= 100000;
    for (uint32_t i = 0; i < NUM_LEDS; i++) {
        led_data[i * BYTES_PER_LED + RED_CHANNEL] = (count + i) % 0xFF;
        led_data[i * BYTES_PER_LED + GREEN_CHANNEL] = (count + i + 0x55) % 0xFF;
        led_data[i * BYTES_PER_LED + BLUE_CHANNEL] = (count + i + 0xAA) % 0xFF;
    }

    /*for (uint32_t i = 0; i < NUM_LEDS; i++) {
        uint8_t value = NUM_LEDS
        if (i % 2 == 0) {
            led_data[i * BYTES_PER_LED + RED_CHANNEL] = value;
            led_data[i * BYTES_PER_LED + GREEN_CHANNEL] = value;
            led_data[i * BYTES_PER_LED + BLUE_CHANNEL] = 0xFF;
        }
        else {
            led_data[i * BYTES_PER_LED + RED_CHANNEL] = 0;
            led_data[i * BYTES_PER_LED + GREEN_CHANNEL] = 0;
            led_data[i * BYTES_PER_LED + BLUE_CHANNEL] = 0;
        }
    }*/

    /*for (int i = 0; i < NUM_LEDS * 3; i++) {
        led_data[i] = 0;
    }
    led_data[(2 * 3) + BLUE_CHANNEL] = 0xFF;*/
}

// ISR function to handle sending the data to the LEDs and loading the next buffer of data to send
bool IRAM_ATTR advance_frame(gptimer_handle_t timer, const gptimer_alarm_event_data_t* edata, void* user_ctx) {
    send_led_data();
    load_next_frame_buffer();
    frame_count++;
    pin_set_level(17, (int32_t)frame_count % 2);
    return true;
}

// Main function containing timer setup and execution
_Noreturn void app_main(void)
{
    ESP_LOGI(TAG, "Startup - initialize data out pin, frame timer, and RMT subsystem");
    pin_reset(DATA_OUT_PIN);
    pin_output(DATA_OUT_PIN, true);
    pin_reset(17);
    pin_output(17, true);

    init_rmt();

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
