# Car

I bought a Pi-controlled RC car only to find that it didn't work, might as well rewrite it.

Original product code: https://github.com/UCTRONICS/UCTRONICS_Smart_Robot_Car_RaspberryPi

### Later TODO: Test out OpenCV and see what data I can get from it
- Figuring out distance and then using that and the ultrasonic sensor's distance could be a starting point.
- Recognizing certain objects/images could be another task.
- Generating an internal representation of the area (mapping) would be cool


#### New plan: 
Arduino UNO with the Adafruit motor shield V2 can run the periphial controls.
This includes:
* Ultrasonic sensor
* All four wheel motors
* Pan and tilt servos

Raspberry Pi will take pictures and somehow interpret information from them with OpenCV.
Unsure if this will be done on the Pi itself or farmed out to another computer via Wifi.

The two boards can communicate either via serial or I2C, I haven't decided yet. The Pi will
have to be the one sending commands to the Arduino for movement and will be reading in data
from the ultrasonic sensor.
-----------------------------------

Raspberry pi dependencies:
* WiringPi: https://github.com/WiringPi/WiringPi

* ws2811 library: https://github.com/jgarff/rpi_ws281x

I should probably use some of CMake's dependency management for these but
it's just easier to grab the built libs from the Pi. The ws2811 lib was built
from source on the pi.


OpenCV may be a dependency but I have not made use of it yet.

-----------------------------------

Arduino dependencies:
* Adafruit Arduino Motor shield v2 library: https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library

* Arduino servo library: https://github.com/arduino-libraries/Servo

The easiest way for me to deal with compilation is to use the Arduino Extension for VS-Code which is being
maintained by Microsoft. I could also just use the Arduino IDE itself but that's a bit more annoying. As for
managing these Arduino dependencies, I'd rather just let the extension run all of the dependency management
and build management.