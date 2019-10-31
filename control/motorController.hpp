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

        void panLeft();
        void panRight();
        void tiltDown();
        void tiltUp();
    private:
        Servo    m_panServo  {UCTronicsPins::PanServo};
        Servo    m_tiltServo {UCTronicsPins::TiltServo};
        PwmWheel m_RearRight {UCTronicsPins::PwmWheelRR};
        PwmWheel m_RearLeft  {UCTronicsPins::PwmWheelRl};
        PwmWheel m_FrontRight{UCTronicsPins::PwmWheelFR};
        PwmWheel m_FrontLeft {UCTronicsPins::PwmWheelFL};
        void outputAllWheelCommands(std::bitset<8> motorCommands);
};


}


#endif