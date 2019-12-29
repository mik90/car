#!/bin/bash
if [[ -d /home/mike/Repos/Car ]]; then
	LOCAL_CAR_DIR=/home/mike/Repos/Car
else
	echo "Unsure where the car repository is, exiting..."
	exit
fi

PI_IP_ADDR=192.168.0.169
REMOTE_CAR_DIR=/home/pi/Repos/Car/

REMOTE=pi@$PI_IP_ADDR:$REMOTE_CAR_DIR/images

# Copy from remote to local
rsync -zavh $REMOTE $LOCAL_CAR_DIR
