#!/bin/bash

if [ -z $ARDUINO_INSTALL ]; then
# Set var if it's unset
ARDUINO_INSTALL=/usr/share/arduino
fi

LIBRARIES_DIR=$ARDUINO_INSTALL/hardware/arduino/avr/libraries

DIRECTORIES="-hardware $ARDUINO_INSTALL/hardware "
DIRECTORIES+="-tools $ARDUINO_INSTALL/tools "
DIRECTORIES+="-libraries $LIBRARIES_DIR "
# This is how to include messages.hpp
DIRECTORIES+="-libraries $(pwd)/motorController/libraries "
DIRECTORIES+="-build-path $(pwd)/motorController/build "

BOARD="-fqbn arduino:avr:uno"

SOURCES="-compile motorController/motorController.ino"

# Build arduino from command line
arduino-builder $DIRECTORIES $BOARD $SOURCES
