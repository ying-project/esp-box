# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/mzy/esp/esp-idf/components/bootloader/subproject"
  "/Users/mzy/Desktop/ESP-BOX/esp-box/examples/image_display/build/bootloader"
  "/Users/mzy/Desktop/ESP-BOX/esp-box/examples/image_display/build/bootloader-prefix"
  "/Users/mzy/Desktop/ESP-BOX/esp-box/examples/image_display/build/bootloader-prefix/tmp"
  "/Users/mzy/Desktop/ESP-BOX/esp-box/examples/image_display/build/bootloader-prefix/src/bootloader-stamp"
  "/Users/mzy/Desktop/ESP-BOX/esp-box/examples/image_display/build/bootloader-prefix/src"
  "/Users/mzy/Desktop/ESP-BOX/esp-box/examples/image_display/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/mzy/Desktop/ESP-BOX/esp-box/examples/image_display/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/mzy/Desktop/ESP-BOX/esp-box/examples/image_display/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
