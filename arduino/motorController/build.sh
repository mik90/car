#!/bin/bash

if [ ! -d ../motorController ]; then
    echo "Error: PWD is not motorController"
    exit 1
fi

if [ -z $ARDUINO_INSTALL ]; then
    # Set var if it's not already set
    ARDUINO_INSTALL=/usr/share/arduino
fi

DIRECTORIES=""
DIRECTORIES+="-hardware $ARDUINO_INSTALL/hardware "
DIRECTORIES+="-tools $ARDUINO_INSTALL/tools "
DIRECTORIES+="-libraries $HOME/Arduino/libraries "
# This is how to include messages.hpp
DIRECTORIES+="-libraries ./libraries "

mkdir -pv .build
BUILD_PATH="-build-path ${PWD}/.build "
BOARD="-fqbn arduino:avr:uno"
SOURCES="-compile motorController.ino"

# Build arduino from command line
echo "arduino-builder -verbose $DIRECTORIES $BOARD $SOURCES"
arduino-builder -verbose $DIRECTORIES $BUILD_PATH $BOARD $SOURCES
