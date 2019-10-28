#ifndef SERVO_HPP_
#define SERVO_HPP_

#include "common_rpi.hpp"

namespace Car
{

using Degrees = unsigned int;

// remoteserver.c notes:
// Angles go up and down in increments of 50

// Pan Left: angleA goes up, caps out at 2300
// Pan Right: angleA goes down, stops at 300

// Tilt up: angleB goes down, stops at 300
// Tilt down: angleB goes up, stops at 1160
static constexpr Degrees panLeftLimit{2300};
static constexpr Degrees panRightLimit{300};
static constexpr Degrees tiltDownLimit{300};
static constexpr Degrees tiltUpLimit{1160};


class Servo
{
    // TODO Figure out the units on this as they're unlabeled
    public:
        Servo(UCTronicsPins::pin_t pin, Degrees upperLimit,
                                        Degrees lowerLimit);
        void turn(Degrees rotation);
        Degrees m_angle;
    private:
        Degrees m_upperLimit;
        Degrees m_lowerLimit;
        UCTronicsPins::pin_t m_pin;
};

}

#endif