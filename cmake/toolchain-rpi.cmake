# Cross compiler for raspberrypi/tools on Github

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(tools /home/mike/Repos/raspberrypi/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf)
set(CMAKE_SYSROOT  ${tools}/arm-linux-gnueabihf/sysroot)

set(CMAKE_STAGING_PREFIX /home/mike/Repos/car/staging)

set(CMAKE_C_COMPILER  ${tools}/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER  ${tools}/bin/arm-linux-gnueabihf-g++)

# Don't use any programs since we're cross compiling
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Only use libraries and headers found in the cross-compilation environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)