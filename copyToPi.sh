#!/bin/bash
# Copies all needed source files to the Pi
# Really just excludes build stuff and git history

# I'm lazy and have the repo as "Car" on one laptop
# and "car" on another
if [[ -d /home/mike/Repos/car/ ]]; then
	LOCAL_CAR_DIR=/home/mike/Repos/car/
elif [[ -d  /home/mike/Repos/Car/ ]]; then
	LOCAL_CAR_DIR=/home/mike/Repos/Car/
else
	echo "Unsure where the car repository is, exiting..."
	exit
fi

PI_IP_ADDR=192.168.0.169
REMOTE_CAR_DIR=/home/pi/Repos/car/

rsync -zavh --exclude 'build' --exclude '.vscode' --exclude '.git' $LOCAL_CAR_DIR  pi@$PI_IP_ADDR:$REMOTE_CAR_DIR
