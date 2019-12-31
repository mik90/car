#!/bin/bash
# Copies binaries to the pi

SOURCE=$(pwd)
if [ ! -f $SOURCE/arduino/build/motorController.ino.hex ]; then
    echo "motorController.ino.hex was not built! Exiting..."
    exit
elif [ ! -f $SOURCE/build/raspberryPi/cliCarController ]; then
    echo "cliCarController was not built! Exiting..."
    exit
elif [ ! -f $SOURCE/build/raspberryPi/faceCarController ]; then
    echo "faceCarController was not built! Exiting..."
    exit
fi

SOURCE_FILES="$SOURCE/arduino/build/motorController.ino.hex "
SOURCE_FILES+="$SOURCE/build/raspberryPi/cliCarController "
SOURCE_FILES+="$SOURCE/build/raspberryPi/faceCarController "
SOURCE_FILES+="$SOURCE/arduino/arduinoUpload.sh "
SOURCE_FILES+="$SOURCE/raspberryPi/lbpcascade_frontalface_improved.xml "

DEST="pi@192.168.0.169:/home/pi/Repos/Car/"

echo "Sending files with command:"
echo "rsync -zavh $SOURCE_FILES $DEST"
rsync -zavh $SOURCE_FILES $DEST