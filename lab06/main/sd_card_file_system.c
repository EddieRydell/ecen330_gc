#include "sd_card_file_system.h"

#include <string.h>
#include <stdio.h>

#include "errno.h"
#include "sys/stat.h"
#include "dirent.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"

// https://github.com/FalconChristmas/fpp/blob/master/docs/FSEQ_Sequence_File_Format.txt
#define HEADER_SIZE 22
#define OFFSET_CHANNEL_DATA_START 4
#define OFFSET_CHANNEL_COUNT_PER_FRAME 10
#define OFFSET_NUM_FRAMES 14
#define OFFSET_STEP_TIME 18
#define OFFSET_COMPRESSION 20
#define HIGH_NIBBLE(byte) ((byte) >> 4)
#define LOW_NIBBLE(byte) (0x0F & (byte))

#define DEFAULT_MAX_FILES 5
#define DEFAULT_ALLOCATION_UNIT_SIZE (16 * 1024)

#define SD_CS_PIN 22
#define SD_MOSI_PIN 23
#define SD_MISO_PIN 19
#define SD_SCK_PIN 18

#define RED_CHANNEL 0
#define GREEN_CHANNEL 2
#define BLUE_CHANNEL 1

#define MOUNT_POINT "/sdcard"

static const char* TAG = "SD CARD FILE SYSTEM";

// Helper function to list the contents of a directory on the sd card specified by path
void list_directory(const char* path) {
    ESP_LOGI(TAG, "Listing directory contents of %s", MOUNT_POINT);
    DIR* dir = opendir(path);
    if (dir == NULL) {
        ESP_LOGE(TAG, "Failed to open directory: %s", strerror(errno));
        return;
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        ESP_LOGI(TAG, "Found file: %s", entry->d_name);
    }
    closedir(dir);
}

// Initialize SD card to read files from it
void init_sd_card() {
    ESP_LOGI(TAG, "Initializing SD card");
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
            .format_if_mount_failed = false,
            .max_files = DEFAULT_MAX_FILES,
            .allocation_unit_size = DEFAULT_ALLOCATION_UNIT_SIZE // DEFAULT_ALLOCATION_UNIT_SIZE
    };

    esp_err_t ret;
    sdmmc_card_t* card;
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    spi_bus_config_t bus_cfg = {
            .mosi_io_num = SD_MOSI_PIN,
            .miso_io_num = SD_MISO_PIN,
            .sclk_io_num = SD_SCK_PIN,
            .quadwp_io_num = -1,
            .quadhd_io_num = -1,
            .max_transfer_sz = 4000,
    };

    ret = spi_bus_initialize(host.slot, &bus_cfg, (spi_dma_chan_t)SDSPI_DEFAULT_DMA);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize bus: %s", esp_err_to_name(ret));
        return;
    }

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = SD_CS_PIN;
    slot_config.host_id = host.slot;
    ESP_LOGI(TAG, "Success initializing SD card");

    ESP_LOGI(TAG, "Mounting filesystem");
    ret = esp_vfs_fat_sdspi_mount(MOUNT_POINT, &host, &slot_config, &mount_config, &card);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                          "If you want the card to be formatted, set the CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
        }
        else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                          "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        return;
    }
    ESP_LOGI(TAG, "Filesystem mounted");
    list_directory(MOUNT_POINT);
}

// Open filename and parse the header
// Returns a fseq_sequence_t initialized based on the header
// This function advances sequence_file to be at the beginning of the actual LED data
fseq_sequence_t open_and_parse_fseq_file(const char* filename) {
    fseq_sequence_t result;
    char file_path[256]; // Ensure this buffer is large enough to hold the full path
    snprintf(file_path, sizeof(file_path), "%s/%s", MOUNT_POINT, filename);
    ESP_LOGI(TAG, "Opening file %s", file_path);

    result.sequence_file = fopen(file_path, "rb");
    if (result.sequence_file == NULL) {
        ESP_LOGE(TAG, "Failed to open file: %s", strerror(errno));
        struct stat st;
        if (stat(file_path, &st) == 0) {
            ESP_LOGI(TAG, "File exists. Permissions: %lo", st.st_mode);
        }
        else {
            ESP_LOGE(TAG, "Stat failed: %s", strerror(errno));
        }
    }
    ESP_LOGI(TAG, "File opened successfully");

    ESP_LOGI(TAG, "Parsing .fseq file header");
    uint8_t header_data[HEADER_SIZE];
    size_t bytes_read = fread(header_data, 1, sizeof(header_data), result.sequence_file);
    if (bytes_read < sizeof(header_data)) {
        if (feof(result.sequence_file)) {
            ESP_LOGI(TAG, "Reached end of file.");
        }
        else if (ferror(result.sequence_file)) {
            ESP_LOGE(TAG, "Error reading file.");
        }
    }
    result.channel_data_offset = *(uint16_t*)(header_data + OFFSET_CHANNEL_DATA_START);
    result.channel_count_per_frame = *(uint32_t*)(header_data + OFFSET_CHANNEL_COUNT_PER_FRAME);
    result.num_frames = *(uint32_t*)(header_data + OFFSET_NUM_FRAMES);
    result.step_time_ms = *(header_data + OFFSET_STEP_TIME);
    result.compression_type = HIGH_NIBBLE(*(header_data + OFFSET_COMPRESSION));
    result.total_compression_blocks = ((uint16_t)LOW_NIBBLE(*(header_data + OFFSET_COMPRESSION)) << 8) | *(header_data + OFFSET_COMPRESSION + 1);

    // Move the file pointer to the data location
    if (fseek(result.sequence_file, result.channel_data_offset, SEEK_SET) != 0) {
        ESP_LOGE(TAG, "Error seeking to data location.");
    }
    return result;
}
