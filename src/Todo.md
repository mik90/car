
Todo right now:
-----------------------------------
* Implement motor control
* Implement manual control mechanism
* Test sensor and motor init
* Test manual control via ssh and cli
* Move lights and buzzer into separate area

Todo in general:
-----------------------------------
* GoogleTest unit testing framework
* Replace raw pointer usage with smart pointers where possible
* Create host-side client to send commands in common message format
* Use something cleaner than C's socket api. Can use Boost ASIO or find some socket wrapper library

### Async actions
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
* Control server
* Mailbox interface

* ws2811 LED interface

* Motor drivers
* Pulse code modulation
* Pulse width modulation
* Clock

* GPIO header
* Direct Memory access
* Rpi hardware interface