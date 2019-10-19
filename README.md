# Car

I bought some Pi-controlled RC car only to find that it didn't work, might as well rewrite it.

Original product code: https://github.com/UCTRONICS/UCTRONICS_Smart_Robot_Car_RaspberryPi

Currently depends on:

* WiringPi: https://github.com/WiringPi/WiringPi

* ws2811 library: https://github.com/jgarff/rpi_ws281x


Todo:
-----------------------------------
* Implement pan/tilt servo control
* Possibly re-implement IR remote handling for easy control
* Add better wheel speed control
* Use a better logging method
* Use Boost's (or other) tcp library for user interface 
* Make manager for dealing with all of the periphial inputs
* Parse camera input
* Find way to use camera for obstacle detection


#### This all was in the UCTRONICS repo, but it's actually just jgarff/ws2811
* Control server
* Mailbox interface
* ws2811 LED interface
* Pulse code modulation
* Pulse width modulation
* Clock
* GPIO header file
* Direct Memory access