#!/bin/bash

# TODO On the gentoo thinkpad, I need to add uvcvideo module to kernel in order to use the webcam

# This is the location on Gentoo, unsure where it'll be on Arch
if [ ! -f /usr/share/opencv4/haarcascades/haarcascade_eye_tree_eyeglasses.xml ] || [ ! -f /usr/share/opencv4/haarcascades/haarcascade_frontalface_alt.xml ]
then
    echo "ERROR: Could not find haarcascade xmls, exiting..."
    exit
fi

../build/opencv/faceRecognizer --camera=0 \
                               --eyes_cascade=/usr/share/opencv4/haarcascades/haarcascade_eye_tree_eyeglasses.xml \
                               --face_cascade=/usr/share/opencv4/haarcascades/haarcascade_frontalface_alt.xml