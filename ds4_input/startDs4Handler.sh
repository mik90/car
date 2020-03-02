#!/bin/bash

#export RUST_BACKTRACE=full

if [ "$1" == "-release" ]; then
    BUILD_TYPE=release
else
    BUILD_TYPE=debug
fi

# Look for the device ID before searching for it in by-path
DEFAULT_PATH=/dev/input/by-id/usb-Sony_Computer_Entertainment_Wireless_Controller-event-joystick

if [ ! -e $DEFAULT_PATH ]; then
    search_result=$(find /dev/input/by-path/*event-joystick)
    result_count=$(wc -w <<< $search_result)
    # Found only a single result and it's a file
    if [ $result_count == 1 ] && [ -e $search_result ]; then
        EVENT_FILE=$search_result
    else
        # Couldn't find anything :(
        SCRIPT_NAME=`basename "$0"`
        echo "${SCRIPT_NAME}: Could not find Dualshock 4 device, exiting..."
        exit 1
    fi
else
    # The default path is a file
    EVENT_FILE=$DEFAULT_PATH
fi

./target/$BUILD_TYPE/ds4_input $EVENT_FILE
