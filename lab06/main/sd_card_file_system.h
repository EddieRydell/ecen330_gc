#ifndef LAB06_SD_CARD_FILE_SYSTEM_H
#define LAB06_SD_CARD_FILE_SYSTEM_H

#include <stdio.h>

typedef struct {
    uint16_t channel_data_offset; // offset to the start of actual channel data
    uint32_t channel_count_per_frame;
    uint32_t num_frames; // total number of frames in sequence
    uint8_t step_time_ms;
    enum {UNCOMPRESSED = 0, ZSTD, GZIP} compression_type; // default UNCOMPRESSED. Compression not supported currently
    uint16_t total_compression_blocks;

    FILE* sequence_file;
} fseq_sequence_t;

// Initialize SD card and mount filesystem
void init_sd_card();

// Open filename and parse the header
// Returns a fseq_sequence_t initialized based on the header
// This function advances sequence_file to be at the beginning of the actual LED data
fseq_sequence_t open_and_parse_fseq_file(const char* filename);


#endif //LAB06_SD_CARD_FILE_SYSTEM_H
