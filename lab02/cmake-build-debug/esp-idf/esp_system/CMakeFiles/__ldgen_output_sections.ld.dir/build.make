# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2023.3.4\bin\cmake\win\x64\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2023.3.4\bin\cmake\win\x64\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\eddie\CLionProjects\ecen330_gc\lab02

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\eddie\CLionProjects\ecen330_gc\lab02\cmake-build-debug

# Utility rule file for __ldgen_output_sections.ld.

# Include any custom commands dependencies for this target.
include esp-idf/esp_system/CMakeFiles/__ldgen_output_sections.ld.dir/compiler_depend.make

# Include the progress variables for this target.
include esp-idf/esp_system/CMakeFiles/__ldgen_output_sections.ld.dir/progress.make

esp-idf/esp_system/CMakeFiles/__ldgen_output_sections.ld: esp-idf/esp_system/ld/sections.ld

esp-idf/esp_system/ld/sections.ld: esp-idf/esp_system/ld/sections.ld.in
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/xtensa/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/esp_driver_gpio/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/esp_pm/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/esp_mm/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/spi_flash/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/esp_system/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/esp_system/app.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/esp_common/common.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/esp_common/soc.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/esp_rom/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/hal/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/log/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/heap/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/soc/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/esp_hw_support/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/esp_hw_support/dma/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/freertos/linker_common.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/freertos/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/newlib/newlib.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/newlib/system_libs.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/esp_ringbuf/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/esp_driver_uart/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/vfs/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/esp_driver_pcnt/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/esp_driver_gptimer/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/esp_driver_spi/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/esp_driver_mcpwm/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/esp_driver_ana_cmpr/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/esp_driver_dac/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/esp_driver_rmt/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/esp_driver_sdm/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/esp_driver_i2c/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/esp_driver_ledc/linker.lf
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/esp/esp-idf/components/driver/twai/linker.lf
esp-idf/esp_system/ld/sections.ld: esp-idf/xtensa/libxtensa.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_driver_gpio/libesp_driver_gpio.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_timer/libesp_timer.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_pm/libesp_pm.a
esp-idf/esp_system/ld/sections.ld: esp-idf/mbedtls/libmbedtls.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_app_format/libesp_app_format.a
esp-idf/esp_system/ld/sections.ld: esp-idf/efuse/libefuse.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_bootloader_format/libesp_bootloader_format.a
esp-idf/esp_system/ld/sections.ld: esp-idf/bootloader_support/libbootloader_support.a
esp-idf/esp_system/ld/sections.ld: esp-idf/app_update/libapp_update.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_mm/libesp_mm.a
esp-idf/esp_system/ld/sections.ld: esp-idf/spi_flash/libspi_flash.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_system/libesp_system.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_common/libesp_common.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_rom/libesp_rom.a
esp-idf/esp_system/ld/sections.ld: esp-idf/hal/libhal.a
esp-idf/esp_system/ld/sections.ld: esp-idf/log/liblog.a
esp-idf/esp_system/ld/sections.ld: esp-idf/heap/libheap.a
esp-idf/esp_system/ld/sections.ld: esp-idf/soc/libsoc.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_hw_support/libesp_hw_support.a
esp-idf/esp_system/ld/sections.ld: esp-idf/freertos/libfreertos.a
esp-idf/esp_system/ld/sections.ld: esp-idf/newlib/libnewlib.a
esp-idf/esp_system/ld/sections.ld: esp-idf/pthread/libpthread.a
esp-idf/esp_system/ld/sections.ld: esp-idf/cxx/libcxx.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_partition/libesp_partition.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_ringbuf/libesp_ringbuf.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_driver_uart/libesp_driver_uart.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_vfs_console/libesp_vfs_console.a
esp-idf/esp_system/ld/sections.ld: esp-idf/vfs/libvfs.a
esp-idf/esp_system/ld/sections.ld: esp-idf/spiffs/libspiffs.a
esp-idf/esp_system/ld/sections.ld: esp-idf/main/libmain.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_driver_pcnt/libesp_driver_pcnt.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_driver_gptimer/libesp_driver_gptimer.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_driver_spi/libesp_driver_spi.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_driver_mcpwm/libesp_driver_mcpwm.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_driver_i2s/libesp_driver_i2s.a
esp-idf/esp_system/ld/sections.ld: esp-idf/sdmmc/libsdmmc.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_driver_sdmmc/libesp_driver_sdmmc.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_driver_sdspi/libesp_driver_sdspi.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_driver_sdio/libesp_driver_sdio.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_driver_dac/libesp_driver_dac.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_driver_rmt/libesp_driver_rmt.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_driver_sdm/libesp_driver_sdm.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_driver_i2c/libesp_driver_i2c.a
esp-idf/esp_system/ld/sections.ld: esp-idf/esp_driver_ledc/libesp_driver_ledc.a
esp-idf/esp_system/ld/sections.ld: esp-idf/driver/libdriver.a
esp-idf/esp_system/ld/sections.ld: esp-idf/lcd/liblcd.a
esp-idf/esp_system/ld/sections.ld: esp-idf/pin/libpin.a
esp-idf/esp_system/ld/sections.ld: C:/Users/eddie/CLionProjects/ecen330_gc/lab02/sdkconfig
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=C:\Users\eddie\CLionProjects\ecen330_gc\lab02\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating ld/sections.ld"
	cd /d C:\Users\eddie\CLionProjects\ecen330_gc\lab02\cmake-build-debug\esp-idf\esp_system && python C:/Users/eddie/esp/esp-idf/tools/ldgen/ldgen.py --config C:/Users/eddie/CLionProjects/ecen330_gc/lab02/sdkconfig --fragments-list C:/Users/eddie/esp/esp-idf/components/xtensa/linker.lf;C:/Users/eddie/esp/esp-idf/components/esp_driver_gpio/linker.lf;C:/Users/eddie/esp/esp-idf/components/esp_pm/linker.lf;C:/Users/eddie/esp/esp-idf/components/esp_mm/linker.lf;C:/Users/eddie/esp/esp-idf/components/spi_flash/linker.lf;C:/Users/eddie/esp/esp-idf/components/esp_system/linker.lf;C:/Users/eddie/esp/esp-idf/components/esp_system/app.lf;C:/Users/eddie/esp/esp-idf/components/esp_common/common.lf;C:/Users/eddie/esp/esp-idf/components/esp_common/soc.lf;C:/Users/eddie/esp/esp-idf/components/esp_rom/linker.lf;C:/Users/eddie/esp/esp-idf/components/hal/linker.lf;C:/Users/eddie/esp/esp-idf/components/log/linker.lf;C:/Users/eddie/esp/esp-idf/components/heap/linker.lf;C:/Users/eddie/esp/esp-idf/components/soc/linker.lf;C:/Users/eddie/esp/esp-idf/components/esp_hw_support/linker.lf;C:/Users/eddie/esp/esp-idf/components/esp_hw_support/dma/linker.lf;C:/Users/eddie/esp/esp-idf/components/freertos/linker_common.lf;C:/Users/eddie/esp/esp-idf/components/freertos/linker.lf;C:/Users/eddie/esp/esp-idf/components/newlib/newlib.lf;C:/Users/eddie/esp/esp-idf/components/newlib/system_libs.lf;C:/Users/eddie/esp/esp-idf/components/esp_ringbuf/linker.lf;C:/Users/eddie/esp/esp-idf/components/esp_driver_uart/linker.lf;C:/Users/eddie/esp/esp-idf/components/vfs/linker.lf;C:/Users/eddie/esp/esp-idf/components/esp_driver_pcnt/linker.lf;C:/Users/eddie/esp/esp-idf/components/esp_driver_gptimer/linker.lf;C:/Users/eddie/esp/esp-idf/components/esp_driver_spi/linker.lf;C:/Users/eddie/esp/esp-idf/components/esp_driver_mcpwm/linker.lf;C:/Users/eddie/esp/esp-idf/components/esp_driver_ana_cmpr/linker.lf;C:/Users/eddie/esp/esp-idf/components/esp_driver_dac/linker.lf;C:/Users/eddie/esp/esp-idf/components/esp_driver_rmt/linker.lf;C:/Users/eddie/esp/esp-idf/components/esp_driver_sdm/linker.lf;C:/Users/eddie/esp/esp-idf/components/esp_driver_i2c/linker.lf;C:/Users/eddie/esp/esp-idf/components/esp_driver_ledc/linker.lf;C:/Users/eddie/esp/esp-idf/components/driver/twai/linker.lf --input C:/Users/eddie/CLionProjects/ecen330_gc/lab02/cmake-build-debug/esp-idf/esp_system/ld/sections.ld.in --output C:/Users/eddie/CLionProjects/ecen330_gc/lab02/cmake-build-debug/esp-idf/esp_system/ld/sections.ld --kconfig C:/Users/eddie/esp/esp-idf/Kconfig --env-file C:/Users/eddie/CLionProjects/ecen330_gc/lab02/cmake-build-debug/config.env --libraries-file C:/Users/eddie/CLionProjects/ecen330_gc/lab02/cmake-build-debug/ldgen_libraries --objdump "C:/Program Files/JetBrains/CLion 2023.3.4/bin/mingw/bin/objdump.exe"

esp-idf/esp_system/ld/sections.ld.in: C:/Users/eddie/esp/esp-idf/components/esp_system/ld/esp32/sections.ld.in
esp-idf/esp_system/ld/sections.ld.in: config/sdkconfig.h
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=C:\Users\eddie\CLionProjects\ecen330_gc\lab02\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating C:/Users/eddie/CLionProjects/ecen330_gc/lab02/cmake-build-debug/esp-idf/esp_system/ld/sections.ld.in linker script..."
	cd /d C:\Users\eddie\CLionProjects\ecen330_gc\lab02\cmake-build-debug\esp-idf\esp_system && "C:\Program Files\JetBrains\CLion 2023.3.4\bin\cmake\win\x64\bin\cmake.exe" -DCC=C:/Users/eddie/.espressif/tools/xtensa-esp-elf/esp-13.2.0_20240305/xtensa-esp-elf/bin/xtensa-esp32-elf-gcc.exe -DSOURCE=C:/Users/eddie/esp/esp-idf/components/esp_system/ld/esp32/sections.ld.in -DTARGET=C:/Users/eddie/CLionProjects/ecen330_gc/lab02/cmake-build-debug/esp-idf/esp_system/ld/sections.ld.in -DCONFIG_DIR=C:/Users/eddie/CLionProjects/ecen330_gc/lab02/cmake-build-debug/config -DLD_DIR=C:/Users/eddie/esp/esp-idf/components/esp_system/ld -P C:/Users/eddie/CLionProjects/ecen330_gc/lab02/cmake-build-debug/esp-idf/esp_system/ld/linker_script_generator.cmake

__ldgen_output_sections.ld: esp-idf/esp_system/CMakeFiles/__ldgen_output_sections.ld
__ldgen_output_sections.ld: esp-idf/esp_system/ld/sections.ld
__ldgen_output_sections.ld: esp-idf/esp_system/ld/sections.ld.in
__ldgen_output_sections.ld: esp-idf/esp_system/CMakeFiles/__ldgen_output_sections.ld.dir/build.make
.PHONY : __ldgen_output_sections.ld

# Rule to build all files generated by this target.
esp-idf/esp_system/CMakeFiles/__ldgen_output_sections.ld.dir/build: __ldgen_output_sections.ld
.PHONY : esp-idf/esp_system/CMakeFiles/__ldgen_output_sections.ld.dir/build

esp-idf/esp_system/CMakeFiles/__ldgen_output_sections.ld.dir/clean:
	cd /d C:\Users\eddie\CLionProjects\ecen330_gc\lab02\cmake-build-debug\esp-idf\esp_system && $(CMAKE_COMMAND) -P CMakeFiles\__ldgen_output_sections.ld.dir\cmake_clean.cmake
.PHONY : esp-idf/esp_system/CMakeFiles/__ldgen_output_sections.ld.dir/clean

esp-idf/esp_system/CMakeFiles/__ldgen_output_sections.ld.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\eddie\CLionProjects\ecen330_gc\lab02 C:\Users\eddie\esp\esp-idf\components\esp_system C:\Users\eddie\CLionProjects\ecen330_gc\lab02\cmake-build-debug C:\Users\eddie\CLionProjects\ecen330_gc\lab02\cmake-build-debug\esp-idf\esp_system C:\Users\eddie\CLionProjects\ecen330_gc\lab02\cmake-build-debug\esp-idf\esp_system\CMakeFiles\__ldgen_output_sections.ld.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : esp-idf/esp_system/CMakeFiles/__ldgen_output_sections.ld.dir/depend
