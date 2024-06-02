#ifndef LAB06_SD_CARD_FILE_SYSTEM_H
#define LAB06_SD_CARD_FILE_SYSTEM_H

#define MOUNT_POINT "/sdcard"

#include <stdio.h>

// Function to list the contents of a directory on the sd card specified by path
void list_directory(const char* path);

// Initialize SD card and mount filesystem
void init_sd_card();

#endif //LAB06_SD_CARD_FILE_SYSTEM_H
