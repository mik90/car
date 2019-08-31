#include "sensors.hpp"

int main(int argc, char** argv)
{
    // Creates memory mapped GPIO 
    Car::Sensors sensors;

    // Allocates matrix for LED data
    Car::RGB lights;

    // TODO Run the servos a bit during init? unsure if needed

    // TODO Init socket

    // TODO Communicate with client, perform commands

    // Async threads:
    // Get input from ultrasonic sensor
    // Get input from infrared sensor

    // Trigger car motion


    return 0;
}