#include "joy.h"

#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"

#define NUM_SAMPLES 5

static const char* TAG = "joy";
adc_oneshot_unit_handle_t adc1_handle;
static int32_t center_x, center_y;

int32_t joy_init(void) {
    adc_oneshot_unit_init_cfg_t init_config1 = {
            .unit_id = ADC_UNIT_1,
            .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    adc_oneshot_chan_cfg_t config = {
            .bitwidth = ADC_BITWIDTH_DEFAULT,
            .atten = ADC_ATTEN_DB_12,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_6, &config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_7, &config));

    int32_t sum_x = 0, sum_y = 0;
    for (uint8_t i = 0; i < NUM_SAMPLES; i++) {
        int32_t current_x = 0;
        int32_t current_y = 0;
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_6, (int*)&current_x));
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_7, (int*)&current_y));
        sum_x += current_x;
        sum_y += current_y;
    }
    center_x = sum_x / NUM_SAMPLES;
    center_y = sum_y / NUM_SAMPLES;

    return 0;
}

int32_t joy_deinit(void) {
    if (adc1_handle != NULL) {
        ESP_ERROR_CHECK(adc_oneshot_del_unit(adc1_handle));
        return 0;
    }
    else return ERR_ADC1_HANDLE_NULL;
}

void joy_get_displacement(int32_t *dcx, int32_t *dcy) {
    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_6, (int*)dcx));
    ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %ld", ADC_UNIT_1 + 1, ADC_CHANNEL_6, *dcx);

    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_7, (int*)dcy));
    ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %ld", ADC_UNIT_1 + 1, ADC_CHANNEL_7, *dcy);

    *dcx -= center_x;
    *dcy -= center_y;
}