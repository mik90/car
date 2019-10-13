# Car

I bought the product below only to find that it doesn't work, might as well rewrite it.

Original product code: https://github.com/UCTRONICS/UCTRONICS_Smart_Robot_Car_RaspberryPi

Currently depends on:

* WiringPi: https://github.com/WiringPi/WiringPi

* ws2811 library: https://github.com/jgarff/rpi_ws281x

Useful references:

* modern-cmake: https://gitlab.com/CLIUtils/modern-cmake/


Todo right now:
-----------------------------------
* Debug motor control, only one wheel is turning
* Use a better logging method
* Use Boost's (or other) tcp library for user interface 


Functionality in Legacy C code:
-----------------------------------
* Motor drivers
* Socket setup


#### This all was in the UCTRONICS repo, but it's actually just jgarff/ws2811
* Control server
* Mailbox interface
* ws2811 LED interface
* Pulse code modulation
* Pulse width modulation
* Clock
* GPIO header file
* Direct Memory access