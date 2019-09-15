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
* Implement motor control
* Implement manual control mechanism
* Test sensor and motor init
* Test manual control via ssh and cli
* Use Boost's tcp library

Todo in general:
-----------------------------------
* Find a lightweight unit testing framework
* Replace raw pointer usage with smart pointers where possible
* Create host-side client to send commands in common message format
* Use something cleaner than C's socket api. Can use Boost ASIO or find some socket wrapper library
* Add an error flag for each module, have the system shut down in case
they get triggered

### Async actions
* Use the C++ futures. 
* Client-side message handler should run on its own. Just wait for input and act on it.
* Server-side should either send in commands manually put in by user or switch the client to an autonomous mode.
* Each affector (pan/tilt and wheels) can have their own threads and run their control loops independently.
* The LED lights and buzzer can do their own thing, wheels are most important for now.

#### Modules:
* Control - Manual mode and automatic mode management
* Comms - Both Pi and Server side. Really just unidirecional, Pi is sink and server is source
* Sensors - parsing input from Pi
* LEDS - writing output to LED


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
* GPIO header
* Direct Memory access