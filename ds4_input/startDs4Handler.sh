#!/bin/bash
# Look for the DS4 joystick event in /dev/input

BY_ID=/dev/input/by-id/usb-Sony_Computer_Entertainment_Wireless_Controller-event-joystick
# Hard-coded PCI path since the id doesn't pop up on my Gentoo thinkpad for some reason
BY_PATH=/dev/input/by-path/pci-0000:00:14.0-usb-0:1:1.0-event-joystick

#export RUST_BACKTRACE=full

if [ "$1" == "-release" ]; then
    BUILD_TYPE=release
else
    BUILD_TYPE=debug
fi

if [ -e $BY_ID ]; then
    ./target/$BUILD_TYPE/ds4_input $BY_ID
elif [ -e $BY_PATH ]; then
    ./target/$BUILD_TYPE/ds4_input $BY_PATH
else
    SCRIPT_NAME=`basename "$0"`
    echo "${SCRIPT_NAME}: Could not find Dualshock 4 device, exiting..."
    exit 1
fi
