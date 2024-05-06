# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/eddie/esp/esp-idf/components/bootloader/subproject"
  "C:/Users/eddie/CLionProjects/ecen330_gc/lab02/cmake-build-debug/bootloader"
  "C:/Users/eddie/CLionProjects/ecen330_gc/lab02/cmake-build-debug/bootloader-prefix"
  "C:/Users/eddie/CLionProjects/ecen330_gc/lab02/cmake-build-debug/bootloader-prefix/tmp"
  "C:/Users/eddie/CLionProjects/ecen330_gc/lab02/cmake-build-debug/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/eddie/CLionProjects/ecen330_gc/lab02/cmake-build-debug/bootloader-prefix/src"
  "C:/Users/eddie/CLionProjects/ecen330_gc/lab02/cmake-build-debug/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/eddie/CLionProjects/ecen330_gc/lab02/cmake-build-debug/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/eddie/CLionProjects/ecen330_gc/lab02/cmake-build-debug/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
