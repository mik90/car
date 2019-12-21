#!/bin/bash
# Copies binaries to the pi

SOURCE=$(pwd)
if [ ! -f $SOURCE/arduino/build/motorController.ino.hex ]; then
    echo "motorController.ino.hex was not built! Exiting..."
    exit
elif [ ! -f $SOURCE/build/bin/carController ]; then
    echo "carController was not built! Exiting..."
    exit
fi

SOURCE_FILES="$SOURCE/arduino/build/motorController.ino.hex $SOURCE/build/bin/carController $SOURCE/arduino/arduinoUpload.sh"

DEST="pi@192.168.0.169:/home/pi/Repos/Car/"

echo "Sending files with command:"
echo "rsync -zavh $SOURCE_FILES $DEST"
rsync -zavh $SOURCE_FILES $DEST
