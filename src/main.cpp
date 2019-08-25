#include "sensors.hpp"

int main(int argc, char** argv)
{
    // Creates memory mapped GPIO 
    Car::Sensors sensors;
    sensors.init_pan_tilt();
    sensors.init_ultrasonic();
    sensors.init_line_reader();
    sensors.init_beep();
    sensors.init_infrared();
    sensors.init_pwm();

    // Allocates matrix for LED data
    Car::RGB lights;

    // TODO Run the servos a bit during init? unsure if needed

    // TODO Init socket

    // TODO Communicate with client, perform commands


    return 0;
}