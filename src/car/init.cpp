#include "wiringPi.h"

#include "car.hpp"

namespace Car
{


Car::Car()
{
    this->initBeep();
    std::cout << "Beeper is initialized" << std::endl;
}

/** @brief Sets pin for the beeper as output and writes it low **/
void Car::initBeep()
{
    pinMode(wPiPins::Beep, OUTPUT);
    digitalWrite(wPiPins::Beep, LOW);
}

}