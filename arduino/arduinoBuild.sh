#!/bin/bash

if [ -z $ARDUINO_INSTALL ]; then
# Set var if it's unset
ARDUINO_INSTALL=/usr/share/arduino
fi

DIRECTORIES=""

DIRECTORIES+="-hardware $ARDUINO_INSTALL/hardware "
DIRECTORIES+="-hardware $HOME/.arduino15/packages "

DIRECTORIES+="-tools $ARDUINO_INSTALL/tools "
DIRECTORIES+="-tools $ARDUINO_INSTALL/tools-builder "
DIRECTORIES+="-tools $HOME/.arduino15/packages "

if [ -d $ARDUINO_INSTALL/hardware/archlinux-arduino ]; then
# Arch will have this directory
DIRECTORIES+="-libraries $ARDUINO_INSTALL/hardware/archlinux-arduino/avr/libraries "
else
# For Gentoo (Default)
DIRECTORIES+="-libraries $ARDUINO_INSTALL/hardware/arduino/avr/libraries "
fi

DIRECTORIES+="-libraries $HOME/Arduino/libraries "
# This is how to include messages.hpp
DIRECTORIES+="-libraries ./motorController/libraries "

mkdir -f build
DIRECTORIES+="-build-path ./build "

BOARD="-fqbn arduino:avr:uno"

SOURCES="-compile motorController/motorController.ino"

# Build arduino from command line
echo "arduino-builder -verbose $DIRECTORIES $BOARD $SOURCES"
arduino-builder -verbose $DIRECTORIES $BOARD $SOURCES

# For uploading manually:
# avrdude -V -F -C /etc/avrdude.conf -p atmega328p -P /dev/ttyACM0 -c arduino -b 115200 -U flash:w:sketch.hex
# avrdude -V -F -C /etc/avrdude.conf -p atmega328p -P /dev/ttyACM0 -c arduino -b 115200 -U flash:w:sketch.hex