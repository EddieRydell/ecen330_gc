#include "interface.h"
#include "sd_card_file_system.h"

#include "esp_vfs.h"
#include "esp_log.h"

#define BACKGROUND_COLOR (rgb565(20, 30, 60))
#define ROW_SPACING 10
#define TEXT_COLOR WHITE
#define SELECTION_COLOR (rgb565(100, 100, 100))

static const char* TAG = "interface";

TFT_t display_config;

static char** directories;
static uint32_t num_directories;
uint32_t directory_index;

void init_display() {
    lcdInit(&display_config);
    lcdFillScreen(&display_config, BACKGROUND_COLOR);
    num_directories = list_directory(MOUNT_POINT);
    lcdFrameDisable(&display_config);

    directories = malloc(sizeof(char*) * num_directories);
    if (directories == NULL) {
        ESP_LOGE(TAG, "Error: malloc failure");
        return;
    }
    for (uint32_t i = 0; i < num_directories; i++) {
        directories[i] = malloc(ESP_VFS_PATH_MAX);
        if (directories[i] == NULL) {
            ESP_LOGE(TAG, "Error: malloc failure");
            return;
        }
    }
    get_directory_names(MOUNT_POINT, directories);
}

void draw_interface() {
    ESP_LOGI(TAG, "Drawing interface");
    lcdFillScreen(&display_config, BACKGROUND_COLOR);
    lcdFillRect(&display_config, 0, (int32_t)directory_index * ROW_SPACING, LCD_W, (int32_t)(directory_index + 1) * ROW_SPACING, SELECTION_COLOR);
    for (uint32_t i = 0; i < num_directories; i++) {
        lcdDrawString(&display_config, 0, (int32_t)i * ROW_SPACING, directories[i], TEXT_COLOR);
        ESP_LOGI(TAG, "%s", directories[i]);
    }
}
