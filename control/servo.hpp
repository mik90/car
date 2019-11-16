#ifndef SERVO_HPP_
#define SERVO_HPP_

#include "commonRpi.hpp"

namespace Car
{

using dutyCycle = unsigned int;

// remoteserver.c notes:
// (Angles?) go up and down in increments of 50
// TODO Figure out the units on this as they're unlabeled
// They seem like pulse width modulation values

static constexpr dutyCycle panLeftLimit {2300};
static constexpr dutyCycle panCenter    {1450};
static constexpr dutyCycle panRightLimit{500};

static constexpr dutyCycle tiltDownLimit{1500};
static constexpr dutyCycle tiltCenter   {2200};
static constexpr dutyCycle tiltUpLimit  {2600};


class Servo
{
    public:
        Servo(wPiPins::pin_t pin);
        void turn(dutyCycle rotation);
        dutyCycle m_angle;
    private:
        wPiPins::pin_t m_pin;
};

}

#endif