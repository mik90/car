#!/bin/bash
# Copies all needed source files to the Pi
# Really just excludes build stuff and git history

PI_IP_ADDR=192.168.0.169
LOCAL_CAR_DIR=/home/mike/Repos/car/
REMOTE_CAR_DIR=/home/pi/Repos/car/

rsync -zavh --exclude 'build' --exclude '.git' $LOCAL_CAR_DIR  pi@$PI_IP_ADDR:$REMOTE_CAR_DIR
