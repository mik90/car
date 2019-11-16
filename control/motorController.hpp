#ifndef MOTOR_CONTROLLER_HPP_
#define MOTOR_CONTROLLER_HPP_

#include <cstdint>
#include <chrono>
#include <bitset>

#include "commonRpi.hpp"
#include "pwmWheel.hpp"
#include "servo.hpp"

namespace Car
{

class MotorController
{
    public:
        MotorController();

        void turnWheels(MotorDir_t leftSideDir, MotorDir_t rightSideDir);
        void setWheelSpeed(PWM::pulseLength pLength);

        void pan(dutyCycle angle);
        void tilt(dutyCycle angle);
    private:
        Servo    m_panServo  {wPiPins::PanServo};
        Servo    m_tiltServo {wPiPins::TiltServo};
        PwmWheel m_RearRight {wPiPins::PwmWheelRR};
        PwmWheel m_RearLeft  {wPiPins::PwmWheelRl};
        PwmWheel m_FrontRight{wPiPins::PwmWheelFR};
        PwmWheel m_FrontLeft {wPiPins::PwmWheelFL};
        void outputAllWheelCommands(std::bitset<8> motorCommands);
};


}


#endif