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

# Include any dependencies generated for this target.
include CMakeFiles/lab02.elf.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/lab02.elf.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/lab02.elf.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lab02.elf.dir/flags.make

project_elf_src_esp32.c:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=C:\Users\eddie\CLionProjects\ecen330_gc\lab02\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating project_elf_src_esp32.c"
	"C:\Program Files\JetBrains\CLion 2023.3.4\bin\cmake\win\x64\bin\cmake.exe" -E touch C:/Users/eddie/CLionProjects/ecen330_gc/lab02/cmake-build-debug/project_elf_src_esp32.c

CMakeFiles/lab02.elf.dir/project_elf_src_esp32.c.obj: CMakeFiles/lab02.elf.dir/flags.make
CMakeFiles/lab02.elf.dir/project_elf_src_esp32.c.obj: project_elf_src_esp32.c
CMakeFiles/lab02.elf.dir/project_elf_src_esp32.c.obj: CMakeFiles/lab02.elf.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\eddie\CLionProjects\ecen330_gc\lab02\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/lab02.elf.dir/project_elf_src_esp32.c.obj"
	C:\Users\eddie\.espressif\tools\xtensa-esp-elf\esp-13.2.0_20240305\xtensa-esp-elf\bin\xtensa-esp32-elf-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/lab02.elf.dir/project_elf_src_esp32.c.obj -MF CMakeFiles\lab02.elf.dir\project_elf_src_esp32.c.obj.d -o CMakeFiles\lab02.elf.dir\project_elf_src_esp32.c.obj -c C:\Users\eddie\CLionProjects\ecen330_gc\lab02\cmake-build-debug\project_elf_src_esp32.c

CMakeFiles/lab02.elf.dir/project_elf_src_esp32.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/lab02.elf.dir/project_elf_src_esp32.c.i"
	C:\Users\eddie\.espressif\tools\xtensa-esp-elf\esp-13.2.0_20240305\xtensa-esp-elf\bin\xtensa-esp32-elf-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\eddie\CLionProjects\ecen330_gc\lab02\cmake-build-debug\project_elf_src_esp32.c > CMakeFiles\lab02.elf.dir\project_elf_src_esp32.c.i

CMakeFiles/lab02.elf.dir/project_elf_src_esp32.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/lab02.elf.dir/project_elf_src_esp32.c.s"
	C:\Users\eddie\.espressif\tools\xtensa-esp-elf\esp-13.2.0_20240305\xtensa-esp-elf\bin\xtensa-esp32-elf-gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\eddie\CLionProjects\ecen330_gc\lab02\cmake-build-debug\project_elf_src_esp32.c -o CMakeFiles\lab02.elf.dir\project_elf_src_esp32.c.s

# Object files for target lab02.elf
lab02_elf_OBJECTS = \
"CMakeFiles/lab02.elf.dir/project_elf_src_esp32.c.obj"

# External object files for target lab02.elf
lab02_elf_EXTERNAL_OBJECTS =

lab02.elf: CMakeFiles/lab02.elf.dir/project_elf_src_esp32.c.obj
lab02.elf: CMakeFiles/lab02.elf.dir/build.make
lab02.elf: esp-idf/xtensa/libxtensa.a
lab02.elf: esp-idf/esp_driver_gpio/libesp_driver_gpio.a
lab02.elf: esp-idf/esp_timer/libesp_timer.a
lab02.elf: esp-idf/esp_pm/libesp_pm.a
lab02.elf: esp-idf/mbedtls/libmbedtls.a
lab02.elf: esp-idf/esp_app_format/libesp_app_format.a
lab02.elf: esp-idf/esp_bootloader_format/libesp_bootloader_format.a
lab02.elf: esp-idf/app_update/libapp_update.a
lab02.elf: esp-idf/esp_partition/libesp_partition.a
lab02.elf: esp-idf/efuse/libefuse.a
lab02.elf: esp-idf/bootloader_support/libbootloader_support.a
lab02.elf: esp-idf/esp_mm/libesp_mm.a
lab02.elf: esp-idf/spi_flash/libspi_flash.a
lab02.elf: esp-idf/esp_system/libesp_system.a
lab02.elf: esp-idf/esp_common/libesp_common.a
lab02.elf: esp-idf/esp_rom/libesp_rom.a
lab02.elf: esp-idf/hal/libhal.a
lab02.elf: esp-idf/log/liblog.a
lab02.elf: esp-idf/heap/libheap.a
lab02.elf: esp-idf/soc/libsoc.a
lab02.elf: esp-idf/esp_hw_support/libesp_hw_support.a
lab02.elf: esp-idf/freertos/libfreertos.a
lab02.elf: esp-idf/newlib/libnewlib.a
lab02.elf: esp-idf/pthread/libpthread.a
lab02.elf: esp-idf/cxx/libcxx.a
lab02.elf: esp-idf/main/libmain.a
lab02.elf: esp-idf/esp_ringbuf/libesp_ringbuf.a
lab02.elf: esp-idf/esp_driver_pcnt/libesp_driver_pcnt.a
lab02.elf: esp-idf/esp_driver_gptimer/libesp_driver_gptimer.a
lab02.elf: esp-idf/esp_driver_spi/libesp_driver_spi.a
lab02.elf: esp-idf/esp_driver_mcpwm/libesp_driver_mcpwm.a
lab02.elf: esp-idf/esp_driver_i2s/libesp_driver_i2s.a
lab02.elf: esp-idf/sdmmc/libsdmmc.a
lab02.elf: esp-idf/esp_driver_sdmmc/libesp_driver_sdmmc.a
lab02.elf: esp-idf/esp_driver_sdspi/libesp_driver_sdspi.a
lab02.elf: esp-idf/esp_driver_sdio/libesp_driver_sdio.a
lab02.elf: esp-idf/esp_driver_dac/libesp_driver_dac.a
lab02.elf: esp-idf/esp_driver_rmt/libesp_driver_rmt.a
lab02.elf: esp-idf/esp_driver_sdm/libesp_driver_sdm.a
lab02.elf: esp-idf/esp_driver_i2c/libesp_driver_i2c.a
lab02.elf: esp-idf/esp_driver_uart/libesp_driver_uart.a
lab02.elf: esp-idf/esp_driver_ledc/libesp_driver_ledc.a
lab02.elf: esp-idf/driver/libdriver.a
lab02.elf: esp-idf/lcd/liblcd.a
lab02.elf: esp-idf/pin/libpin.a
lab02.elf: esp-idf/driver/libdriver.a
lab02.elf: esp-idf/esp_driver_pcnt/libesp_driver_pcnt.a
lab02.elf: esp-idf/esp_driver_gptimer/libesp_driver_gptimer.a
lab02.elf: esp-idf/esp_driver_mcpwm/libesp_driver_mcpwm.a
lab02.elf: esp-idf/esp_driver_sdmmc/libesp_driver_sdmmc.a
lab02.elf: esp-idf/esp_driver_sdspi/libesp_driver_sdspi.a
lab02.elf: esp-idf/esp_driver_spi/libesp_driver_spi.a
lab02.elf: esp-idf/sdmmc/libsdmmc.a
lab02.elf: esp-idf/esp_driver_sdio/libesp_driver_sdio.a
lab02.elf: esp-idf/esp_driver_dac/libesp_driver_dac.a
lab02.elf: esp-idf/esp_driver_i2s/libesp_driver_i2s.a
lab02.elf: esp-idf/esp_driver_rmt/libesp_driver_rmt.a
lab02.elf: esp-idf/esp_driver_sdm/libesp_driver_sdm.a
lab02.elf: esp-idf/esp_driver_i2c/libesp_driver_i2c.a
lab02.elf: esp-idf/esp_driver_uart/libesp_driver_uart.a
lab02.elf: esp-idf/esp_ringbuf/libesp_ringbuf.a
lab02.elf: esp-idf/esp_driver_ledc/libesp_driver_ledc.a
lab02.elf: esp-idf/xtensa/libxtensa.a
lab02.elf: esp-idf/esp_driver_gpio/libesp_driver_gpio.a
lab02.elf: esp-idf/esp_timer/libesp_timer.a
lab02.elf: esp-idf/esp_pm/libesp_pm.a
lab02.elf: esp-idf/mbedtls/libmbedtls.a
lab02.elf: esp-idf/esp_app_format/libesp_app_format.a
lab02.elf: esp-idf/esp_bootloader_format/libesp_bootloader_format.a
lab02.elf: esp-idf/app_update/libapp_update.a
lab02.elf: esp-idf/esp_partition/libesp_partition.a
lab02.elf: esp-idf/efuse/libefuse.a
lab02.elf: esp-idf/bootloader_support/libbootloader_support.a
lab02.elf: esp-idf/esp_mm/libesp_mm.a
lab02.elf: esp-idf/spi_flash/libspi_flash.a
lab02.elf: esp-idf/esp_system/libesp_system.a
lab02.elf: esp-idf/esp_common/libesp_common.a
lab02.elf: esp-idf/esp_rom/libesp_rom.a
lab02.elf: esp-idf/hal/libhal.a
lab02.elf: esp-idf/log/liblog.a
lab02.elf: esp-idf/heap/libheap.a
lab02.elf: esp-idf/soc/libsoc.a
lab02.elf: esp-idf/esp_hw_support/libesp_hw_support.a
lab02.elf: esp-idf/freertos/libfreertos.a
lab02.elf: esp-idf/newlib/libnewlib.a
lab02.elf: esp-idf/pthread/libpthread.a
lab02.elf: esp-idf/cxx/libcxx.a
lab02.elf: esp-idf/mbedtls/mbedtls/library/libmbedtls.a
lab02.elf: esp-idf/mbedtls/mbedtls/library/libmbedcrypto.a
lab02.elf: esp-idf/mbedtls/mbedtls/library/libmbedx509.a
lab02.elf: esp-idf/mbedtls/mbedtls/3rdparty/everest/libeverest.a
lab02.elf: esp-idf/mbedtls/mbedtls/3rdparty/p256-m/libp256m.a
lab02.elf: esp-idf/xtensa/libxtensa.a
lab02.elf: esp-idf/esp_driver_gpio/libesp_driver_gpio.a
lab02.elf: esp-idf/esp_timer/libesp_timer.a
lab02.elf: esp-idf/esp_pm/libesp_pm.a
lab02.elf: esp-idf/mbedtls/libmbedtls.a
lab02.elf: esp-idf/esp_app_format/libesp_app_format.a
lab02.elf: esp-idf/esp_bootloader_format/libesp_bootloader_format.a
lab02.elf: esp-idf/app_update/libapp_update.a
lab02.elf: esp-idf/esp_partition/libesp_partition.a
lab02.elf: esp-idf/efuse/libefuse.a
lab02.elf: esp-idf/bootloader_support/libbootloader_support.a
lab02.elf: esp-idf/esp_mm/libesp_mm.a
lab02.elf: esp-idf/spi_flash/libspi_flash.a
lab02.elf: esp-idf/esp_system/libesp_system.a
lab02.elf: esp-idf/esp_common/libesp_common.a
lab02.elf: esp-idf/esp_rom/libesp_rom.a
lab02.elf: esp-idf/hal/libhal.a
lab02.elf: esp-idf/log/liblog.a
lab02.elf: esp-idf/heap/libheap.a
lab02.elf: esp-idf/soc/libsoc.a
lab02.elf: esp-idf/esp_hw_support/libesp_hw_support.a
lab02.elf: esp-idf/freertos/libfreertos.a
lab02.elf: esp-idf/newlib/libnewlib.a
lab02.elf: esp-idf/pthread/libpthread.a
lab02.elf: esp-idf/cxx/libcxx.a
lab02.elf: esp-idf/mbedtls/mbedtls/library/libmbedtls.a
lab02.elf: esp-idf/mbedtls/mbedtls/library/libmbedcrypto.a
lab02.elf: esp-idf/mbedtls/mbedtls/library/libmbedx509.a
lab02.elf: esp-idf/mbedtls/mbedtls/3rdparty/everest/libeverest.a
lab02.elf: esp-idf/mbedtls/mbedtls/3rdparty/p256-m/libp256m.a
lab02.elf: esp-idf/xtensa/libxtensa.a
lab02.elf: esp-idf/esp_driver_gpio/libesp_driver_gpio.a
lab02.elf: esp-idf/esp_timer/libesp_timer.a
lab02.elf: esp-idf/esp_pm/libesp_pm.a
lab02.elf: esp-idf/mbedtls/libmbedtls.a
lab02.elf: esp-idf/esp_app_format/libesp_app_format.a
lab02.elf: esp-idf/esp_bootloader_format/libesp_bootloader_format.a
lab02.elf: esp-idf/app_update/libapp_update.a
lab02.elf: esp-idf/esp_partition/libesp_partition.a
lab02.elf: esp-idf/efuse/libefuse.a
lab02.elf: esp-idf/bootloader_support/libbootloader_support.a
lab02.elf: esp-idf/esp_mm/libesp_mm.a
lab02.elf: esp-idf/spi_flash/libspi_flash.a
lab02.elf: esp-idf/esp_system/libesp_system.a
lab02.elf: esp-idf/esp_common/libesp_common.a
lab02.elf: esp-idf/esp_rom/libesp_rom.a
lab02.elf: esp-idf/hal/libhal.a
lab02.elf: esp-idf/log/liblog.a
lab02.elf: esp-idf/heap/libheap.a
lab02.elf: esp-idf/soc/libsoc.a
lab02.elf: esp-idf/esp_hw_support/libesp_hw_support.a
lab02.elf: esp-idf/freertos/libfreertos.a
lab02.elf: esp-idf/newlib/libnewlib.a
lab02.elf: esp-idf/pthread/libpthread.a
lab02.elf: esp-idf/cxx/libcxx.a
lab02.elf: esp-idf/mbedtls/mbedtls/library/libmbedtls.a
lab02.elf: esp-idf/mbedtls/mbedtls/library/libmbedcrypto.a
lab02.elf: esp-idf/mbedtls/mbedtls/library/libmbedx509.a
lab02.elf: esp-idf/mbedtls/mbedtls/3rdparty/everest/libeverest.a
lab02.elf: esp-idf/mbedtls/mbedtls/3rdparty/p256-m/libp256m.a
lab02.elf: esp-idf/xtensa/libxtensa.a
lab02.elf: esp-idf/esp_driver_gpio/libesp_driver_gpio.a
lab02.elf: esp-idf/esp_timer/libesp_timer.a
lab02.elf: esp-idf/esp_pm/libesp_pm.a
lab02.elf: esp-idf/mbedtls/libmbedtls.a
lab02.elf: esp-idf/esp_app_format/libesp_app_format.a
lab02.elf: esp-idf/esp_bootloader_format/libesp_bootloader_format.a
lab02.elf: esp-idf/app_update/libapp_update.a
lab02.elf: esp-idf/esp_partition/libesp_partition.a
lab02.elf: esp-idf/efuse/libefuse.a
lab02.elf: esp-idf/bootloader_support/libbootloader_support.a
lab02.elf: esp-idf/esp_mm/libesp_mm.a
lab02.elf: esp-idf/spi_flash/libspi_flash.a
lab02.elf: esp-idf/esp_system/libesp_system.a
lab02.elf: esp-idf/esp_common/libesp_common.a
lab02.elf: esp-idf/esp_rom/libesp_rom.a
lab02.elf: esp-idf/hal/libhal.a
lab02.elf: esp-idf/log/liblog.a
lab02.elf: esp-idf/heap/libheap.a
lab02.elf: esp-idf/soc/libsoc.a
lab02.elf: esp-idf/esp_hw_support/libesp_hw_support.a
lab02.elf: esp-idf/freertos/libfreertos.a
lab02.elf: esp-idf/newlib/libnewlib.a
lab02.elf: esp-idf/pthread/libpthread.a
lab02.elf: esp-idf/cxx/libcxx.a
lab02.elf: esp-idf/mbedtls/mbedtls/library/libmbedtls.a
lab02.elf: esp-idf/mbedtls/mbedtls/library/libmbedcrypto.a
lab02.elf: esp-idf/mbedtls/mbedtls/library/libmbedx509.a
lab02.elf: esp-idf/mbedtls/mbedtls/3rdparty/everest/libeverest.a
lab02.elf: esp-idf/mbedtls/mbedtls/3rdparty/p256-m/libp256m.a
lab02.elf: C:/Users/eddie/esp/esp-idf/components/xtensa/esp32/libxt_hal.a
lab02.elf: esp-idf/pthread/libpthread.a
lab02.elf: esp-idf/newlib/libnewlib.a
lab02.elf: esp-idf/cxx/libcxx.a
lab02.elf: esp-idf/esp_system/ld/memory.ld
lab02.elf: esp-idf/esp_system/ld/sections.ld
lab02.elf: C:/Users/eddie/esp/esp-idf/components/esp_rom/esp32/ld/esp32.rom.ld
lab02.elf: C:/Users/eddie/esp/esp-idf/components/esp_rom/esp32/ld/esp32.rom.api.ld
lab02.elf: C:/Users/eddie/esp/esp-idf/components/esp_rom/esp32/ld/esp32.rom.libgcc.ld
lab02.elf: C:/Users/eddie/esp/esp-idf/components/esp_rom/esp32/ld/esp32.rom.newlib-data.ld
lab02.elf: C:/Users/eddie/esp/esp-idf/components/esp_rom/esp32/ld/esp32.rom.syscalls.ld
lab02.elf: C:/Users/eddie/esp/esp-idf/components/esp_rom/esp32/ld/esp32.rom.newlib-funcs.ld
lab02.elf: C:/Users/eddie/esp/esp-idf/components/soc/esp32/ld/esp32.peripherals.ld
lab02.elf: CMakeFiles/lab02.elf.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\Users\eddie\CLionProjects\ecen330_gc\lab02\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable lab02.elf"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\lab02.elf.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lab02.elf.dir/build: lab02.elf
.PHONY : CMakeFiles/lab02.elf.dir/build

CMakeFiles/lab02.elf.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\lab02.elf.dir\cmake_clean.cmake
.PHONY : CMakeFiles/lab02.elf.dir/clean

CMakeFiles/lab02.elf.dir/depend: project_elf_src_esp32.c
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\eddie\CLionProjects\ecen330_gc\lab02 C:\Users\eddie\CLionProjects\ecen330_gc\lab02 C:\Users\eddie\CLionProjects\ecen330_gc\lab02\cmake-build-debug C:\Users\eddie\CLionProjects\ecen330_gc\lab02\cmake-build-debug C:\Users\eddie\CLionProjects\ecen330_gc\lab02\cmake-build-debug\CMakeFiles\lab02.elf.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/lab02.elf.dir/depend

