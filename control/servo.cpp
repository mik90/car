#include "servo.hpp"

namespace Car
{

Servo::Servo(wPiPins::pin_t pin)
{
    m_pin = pin;
    // Starting angle is 1140
    m_angle = 1140;

    // ---------------
    // For pan:
    // None of the 'angles' work
    // There must be a problem in the servo control logic

    // ---------------
    // For tilt:
    // every OTHER invocation of tilt up/down works
    // 0 is flat down
    // 1140 is flat down

    // 1500 is probably as low as we should go
    // 1700 is like 45 deg
    // 2300 is 90 deg
    // ---------------
    turn(m_angle);
}


void Servo::turn(Degrees rotation)
{
    // TODO wtf was this supposed to be? all these values are arbitrary
    static constexpr unsigned int servoCycles{3};
    static constexpr unsigned int fullDutyCycle{25000};
    static constexpr unsigned int halfDutyCycle{fullDutyCycle / 2};

    // range 2 - 100
    for (auto i = 0; i < servoCycles; i++)
    {
        digitalWrite(m_pin, HIGH);
        delayMicroseconds(rotation);
        digitalWrite(m_pin, LOW);
        delayMicroseconds(fullDutyCycle - rotation);
    }


}

}