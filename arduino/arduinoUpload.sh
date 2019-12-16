#!/bin/bash

avrdude -V -F -C /etc/avrdude.conf \
              -p atmega328p \
              -P /dev/ttyACM0 \
              -c arduino \
              -U flash:w:./build/motorController.ino.hex