#include "servo.hpp"

namespace Car
{

Servo::Servo(UCTronicsPins::pin_t pin)
{
    m_pin = pin;
    // Starting angle is 1140
    m_angle = 1140;
    turn(m_angle);
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