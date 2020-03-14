set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_SYSTEM_Version 1)

set(tools /home/mike/x-tools/armv6l-rpi-linux-gnueabi)

set(CMAKE_SYSROOT ${tools}/armv6l-rpi-linux-gnueabi/sysroot)

set(CMAKE_C_COMPILER ${tools}/bin/armv6l-rpi-linux-gnueabi-gcc)
set(CMAKE_CXX_COMPILER ${tools}/bin/armv6l-rpi-linux-gnueabi-g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)