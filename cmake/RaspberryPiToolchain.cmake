set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_Version 1)

set(CMAKE_C_COMPILER ${CMAKE_CURRENT_SOURCE_DIR}/extern/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${CMAKE_CURRENT_SOURCE_DIR}/extern/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)

set(CMAKE_FIND_ROOTH_PATH ${CMAKE_CURRENT_SOURCE_DIR}/extern/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/sysroot)

set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -O0 -DDEBUG=YES" CACHE STRING "Debug C++ flags")
set(CMAKE_CXX_FLAGS_RELEASE "-O2" CACHE STRING "Release C++ flags")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)