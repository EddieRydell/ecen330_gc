# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.28

# compile C with C:/Users/eddie/.espressif/tools/xtensa-esp-elf/esp-13.2.0_20240305/xtensa-esp-elf/bin/xtensa-esp32-elf-gcc.exe
C_DEFINES = -DESP_PLATFORM -DIDF_VER=\"v5.4-dev-79-gefa15dbd45\" -DMBEDTLS_CONFIG_FILE=\"mbedtls/esp_config.h\" -DSOC_MMU_PAGE_SIZE=CONFIG_MMU_PAGE_SIZE -DSOC_XTAL_FREQ_MHZ=CONFIG_XTAL_FREQ -D_GLIBCXX_HAVE_POSIX_SEMAPHORE -D_GLIBCXX_USE_POSIX_SEMAPHORE -D_GNU_SOURCE -D_POSIX_READER_WRITER_LOCKS

C_INCLUDES = -IC:\Users\eddie\CLionProjects\ecen330_gc\lab02\cmake-build-debug\config -IC:\Users\eddie\esp\esp-idf\components\newlib\platform_include -IC:\Users\eddie\esp\esp-idf\components\freertos\config\include -IC:\Users\eddie\esp\esp-idf\components\freertos\config\include\freertos -IC:\Users\eddie\esp\esp-idf\components\freertos\config\xtensa\include -IC:\Users\eddie\esp\esp-idf\components\freertos\FreeRTOS-Kernel\include -IC:\Users\eddie\esp\esp-idf\components\freertos\FreeRTOS-Kernel\portable\xtensa\include -IC:\Users\eddie\esp\esp-idf\components\freertos\FreeRTOS-Kernel\portable\xtensa\include\freertos -IC:\Users\eddie\esp\esp-idf\components\freertos\esp_additions\include -IC:\Users\eddie\esp\esp-idf\components\esp_hw_support\include -IC:\Users\eddie\esp\esp-idf\components\esp_hw_support\include\soc -IC:\Users\eddie\esp\esp-idf\components\esp_hw_support\include\soc\esp32 -IC:\Users\eddie\esp\esp-idf\components\esp_hw_support\dma\include -IC:\Users\eddie\esp\esp-idf\components\esp_hw_support\ldo\include -IC:\Users\eddie\esp\esp-idf\components\esp_hw_support\port\esp32\. -IC:\Users\eddie\esp\esp-idf\components\esp_hw_support\port\esp32\include -IC:\Users\eddie\esp\esp-idf\components\heap\include -IC:\Users\eddie\esp\esp-idf\components\log\include -IC:\Users\eddie\esp\esp-idf\components\soc\include -IC:\Users\eddie\esp\esp-idf\components\soc\esp32 -IC:\Users\eddie\esp\esp-idf\components\soc\esp32\include -IC:\Users\eddie\esp\esp-idf\components\hal\platform_port\include -IC:\Users\eddie\esp\esp-idf\components\hal\esp32\include -IC:\Users\eddie\esp\esp-idf\components\hal\include -IC:\Users\eddie\esp\esp-idf\components\esp_rom\include -IC:\Users\eddie\esp\esp-idf\components\esp_rom\include\esp32 -IC:\Users\eddie\esp\esp-idf\components\esp_rom\esp32 -IC:\Users\eddie\esp\esp-idf\components\esp_common\include -IC:\Users\eddie\esp\esp-idf\components\esp_system\include -IC:\Users\eddie\esp\esp-idf\components\esp_system\port\soc -IC:\Users\eddie\esp\esp-idf\components\esp_system\port\include\private -IC:\Users\eddie\esp\esp-idf\components\xtensa\esp32\include -IC:\Users\eddie\esp\esp-idf\components\xtensa\include -IC:\Users\eddie\esp\esp-idf\components\xtensa\deprecated_include -IC:\Users\eddie\esp\esp-idf\components\esp_driver_gpio\include -IC:\Users\eddie\esp\esp-idf\components\esp_timer\include -IC:\Users\eddie\esp\esp-idf\components\esp_pm\include -IC:\Users\eddie\esp\esp-idf\components\mbedtls\port\include -IC:\Users\eddie\esp\esp-idf\components\mbedtls\mbedtls\include -IC:\Users\eddie\esp\esp-idf\components\mbedtls\mbedtls\library -IC:\Users\eddie\esp\esp-idf\components\mbedtls\esp_crt_bundle\include -IC:\Users\eddie\esp\esp-idf\components\mbedtls\mbedtls\3rdparty\everest\include -IC:\Users\eddie\esp\esp-idf\components\mbedtls\mbedtls\3rdparty\p256-m -IC:\Users\eddie\esp\esp-idf\components\mbedtls\mbedtls\3rdparty\p256-m\p256-m -IC:\Users\eddie\esp\esp-idf\components\esp_app_format\include -IC:\Users\eddie\esp\esp-idf\components\efuse\include -IC:\Users\eddie\esp\esp-idf\components\efuse\esp32\include -IC:\Users\eddie\esp\esp-idf\components\esp_bootloader_format\include -IC:\Users\eddie\esp\esp-idf\components\bootloader_support\include -IC:\Users\eddie\esp\esp-idf\components\bootloader_support\bootloader_flash\include -IC:\Users\eddie\esp\esp-idf\components\app_update\include -IC:\Users\eddie\esp\esp-idf\components\esp_partition\include -IC:\Users\eddie\esp\esp-idf\components\esp_mm\include -IC:\Users\eddie\esp\esp-idf\components\spi_flash\include -IC:\Users\eddie\esp\esp-idf\components\pthread\include -IC:\Users\eddie\esp\esp-idf\components\esp_ringbuf\include -IC:\Users\eddie\esp\esp-idf\components\esp_driver_uart\include -IC:\Users\eddie\esp\esp-idf\components\vfs\include -IC:\Users\eddie\esp\esp-idf\components\esp_driver_usb_serial_jtag\include -IC:\Users\eddie\esp\esp-idf\components\esp_vfs_console\include -IC:\Users\eddie\esp\esp-idf\components\spiffs\include -IC:\Users\eddie\esp\esp-idf\components\esp_driver_pcnt\include -IC:\Users\eddie\esp\esp-idf\components\esp_driver_gptimer\include -IC:\Users\eddie\esp\esp-idf\components\esp_driver_spi\include -IC:\Users\eddie\esp\esp-idf\components\esp_driver_mcpwm\include -IC:\Users\eddie\esp\esp-idf\components\esp_driver_ana_cmpr\include -IC:\Users\eddie\esp\esp-idf\components\esp_driver_i2s\include -IC:\Users\eddie\esp\esp-idf\components\sdmmc\include -IC:\Users\eddie\esp\esp-idf\components\esp_driver_sdmmc\include -IC:\Users\eddie\esp\esp-idf\components\esp_driver_sdspi\include -IC:\Users\eddie\esp\esp-idf\components\esp_driver_sdio\include -IC:\Users\eddie\esp\esp-idf\components\esp_driver_dac\include -IC:\Users\eddie\esp\esp-idf\components\esp_driver_rmt\include -IC:\Users\eddie\esp\esp-idf\components\esp_driver_tsens\include -IC:\Users\eddie\esp\esp-idf\components\esp_driver_sdm\include -IC:\Users\eddie\esp\esp-idf\components\esp_driver_i2c\include -IC:\Users\eddie\esp\esp-idf\components\esp_driver_ledc\include -IC:\Users\eddie\esp\esp-idf\components\esp_driver_parlio\include -IC:\Users\eddie\esp\esp-idf\components\driver\deprecated -IC:\Users\eddie\esp\esp-idf\components\driver\i2c\include -IC:\Users\eddie\esp\esp-idf\components\driver\touch_sensor\include -IC:\Users\eddie\esp\esp-idf\components\driver\twai\include -IC:\Users\eddie\esp\esp-idf\components\driver\touch_sensor\esp32\include -IC:\Users\eddie\CLionProjects\ecen330_gc\components\lcd -IC:\Users\eddie\CLionProjects\ecen330_gc\components\pin

C_FLAGS = -mlongcalls -Wno-frame-address  -fno-builtin-memcpy -fno-builtin-memset -fno-builtin-bzero -fno-builtin-stpcpy -fno-builtin-strncpy -fdiagnostics-color=always -ffunction-sections -fdata-sections -Wall -Werror=all -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=unused-but-set-variable -Wno-error=deprecated-declarations -Wextra -Wno-unused-parameter -Wno-sign-compare -Wno-enum-conversion -gdwarf-4 -ggdb -O2 -fmacro-prefix-map=C:/Users/eddie/CLionProjects/ecen330_gc/lab02=. -fmacro-prefix-map=C:/Users/eddie/esp/esp-idf=/IDF -fstrict-volatile-bitfields -fno-jump-tables -fno-tree-switch-conversion -std=gnu17 -Wno-old-style-declaration

