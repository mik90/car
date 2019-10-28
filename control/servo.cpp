#include "servo.hpp"

namespace Car
{

Servo::Servo(UCTronicsPins::pin_t pin, Degrees upperLimit,
                                       Degrees lowerLimit)
{
    m_pin = pin;
    // Default angle is 1140
    m_angle = 1140;
    m_upperLimit = upperLimit;
    m_lowerLimit = lowerLimit;
}


void panLeft(Servo& s)
{
    // Increase angle by 50, cap at 2300
    if ((s.m_angle + 50) < panLeftLimit)
    {
        s.m_angle += 50;
        s.turn(s.m_angle);
    }
}

void Servo::turn(Degrees rotation)
{
    // TODO wtf was this supposed to be? all these values are arbitrary
    static constexpr unsigned int servoCycles{3};
    static constexpr unsigned int fullDutyCycle{25000};

    for (auto i = 0; i < servoCycles; i++)
    {
        RpiInterface::writeGpioBits(1 << m_pin);
        delayMicroseconds(rotation);
        RpiInterface::clearGpioBits(1 << m_pin);
        delayMicroseconds(fullDutyCycle - rotation);
    }


}

}