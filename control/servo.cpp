#include "effectors.hpp"

namespace Car
{
Servo::Servo(UCTronicsPins::pin_t const pin) : m_pin(pin) { }

void Servo::turn(Angle_t dutyCycle)
{
    // TODO wtf was this supposed to be? all these values are arbitrary
    static constexpr unsigned int servoCycles{3};
    static constexpr unsigned int servoPeriod{25000};

    for (auto i = 0; i < servoCycles; i++)
    {
        RpiInterface::writeGpioBits(1 << m_pin);
        delayMicroseconds(dutyCycle);
        RpiInterface::clearGpioBits(1 << m_pin);
        delayMicroseconds(servoPeriod - dutyCycle);
    }


}

}