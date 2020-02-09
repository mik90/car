#!/bin/bash
if [ -f .build/motorController.ino.hex ]; then
    HEX_FILE=".build/motorController.ino.hex"
elif [ -f motorController.ino.hex ]; then
    HEX_FILE="motorController.ino.hex"
else
    echo "cannot find motorController.ino.hex"
    echo "Exiting..."
    exit 1
fi

avrdude -V -F -C /etc/avrdude.conf \
              -p atmega328p \
              -P /dev/ttyACM0 \
              -c arduino \
              -U flash:w:$HEX_FILE