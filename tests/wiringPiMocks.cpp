#include "wiringPiMocks.hpp"

extern WiringPiMocks wiringPiMocks;

void wiringPiSetup()
{
    return wiringPiMocks.wiringPiSetup();
}

void pinMode(int pin, int mode)
{
    return wiringPiMocks.pinMode(pin, mode);
}


void pinMode(int pin, int value)
{
    return wiringPiMocks.digitalWrite(pin, value);
}