#!/bin/bash
# Copies all needed source files to the Pi
# Really just excludes build stuff and git history

SOURCE=./

PI_IP_ADDR=192.168.0.169
REMOTE_CAR_DIR=/home/pi/Repos/car/
DEST=pi@$PI_IP_ADDR:$REMOTE_CAR_DIR

rsync -zavh --exclude 'arduino' --exclude 'build' --exclude '.vscode' --exclude '.git' $SOURCE $DEST
