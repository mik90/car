#!/bin/bash

if [ ! -f /home/pi/Repos/Car/test/haarcascade_eye_tree_eyeglasses.xml ] || [ ! -f /home/pi/Repos/Car/test/haarcascade_frontalface_alt.xml ]
then
    echo "ERROR: Could not find haarcascade xmls, exiting..."
    exit
fi

./build/cameraTest --camera=0 \
             --eyes_cascade=/home/pi/Repos/Car/haarcascade_eye_tree_eyeglasses.xml \
             --face_cascade=/home/pi/Repos/Car/haarcascade_frontalface_alt.xml