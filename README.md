# Car

I bought a Pi-controlled RC car only to find that it didn't work, might as well rewrite it and play around.

Original product: https://github.com/UCTRONICS/UCTRONICS_Smart_Robot_Car_RaspberryPi

After re-writing it multiple times and adding an Adruino board, I have it (somewhat) working.

Its main feature is that it starts moving forward once it sees a face. Face detection is done using OpenCV
with a LBP Cascade classifier. The camera is the Raspberry Pi Camera attachment which is plugged-in by a flat cable.
The motor control is done on a Arduino UNO which has a Motor Shield V2 from
Adafruit. Communication between the two is done by a USB-to-serial cable that plugs into the Pi's usb slot and
connects to the Arduino's serial port.

There's still plenty of issues to be ironed out but I just want to put this aside for now as I've spent way too much
time playing around with it.
* I think it's a bit too heavy now and doesn't exactly move very well. 
* The left-side motors have recently stopped working and I haven't checked if it's a software or hardware issue.
* I could iron-out the serial interface and make it cleaner and less buggy

![Picture of the car](CarPic.jpg)

-----------------------------------

Raspberry pi dependencies:
* OpenCV 4.2.0, fork of the rasperrypi/tools repo: https://github.com/mik90/tools
    - I built this from source since the version in the Raspbian Buster repos is only 3.2.0. It's a bit
      more annoying to add and certainly isn't going to be as stable as 3.2.0 but cross-compiling it with
      CMake isn't all that hard. On the pi, I just had the libraries in a folder and appended to the LD_LIBRARY_PATH
      as a temporary way to get it working.

Arduino dependencies:
* Adafruit Arduino Motor shield v2 library: https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library

* Arduino servo library: https://github.com/arduino-libraries/Servo

The easiest way for me to deal with compilation was to use the Arduino Extension for VS-Code which is being
maintained by Microsoft.