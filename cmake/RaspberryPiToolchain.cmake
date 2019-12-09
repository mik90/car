SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_Version 1)

SET(CMAKE_C_COMPILER ${CMAKE_CURRENT_SOURCE_DIR}/extern/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER ${CMAKE_CURRENT_SOURCE_DIR}/extern/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)

SET(CMAKE_FIND_ROOTH_PATH ${CMAKE_CURRENT_SOURCE_DIR}/extern/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/sysroot)

SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)