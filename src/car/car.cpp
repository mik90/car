#include <iostream>

#include "car.hpp"

namespace Car
{


void Car::moveCar(CarDirection_t carDir)
{
    auto turnLeftSide = [this](MotorDir_t motorDir)
    {
        m_motors.turnDcMotor(BCM::MotorPWM_FL, motorDir);
        m_motors.turnDcMotor(BCM::MotorPWM_RL, motorDir);
    };
    auto turnRightSide = [this](MotorDir_t motorDir)
    {
        m_motors.turnDcMotor(BCM::MotorPWM_FR, motorDir);
        m_motors.turnDcMotor(BCM::MotorPWM_RR, motorDir);
    };

    switch(carDir)
    {
        FORWARD:
            turnLeftSide(MotorDir_t::FORWARD);
            turnRightSide(MotorDir_t::FORWARD);
            break;
        FORWARD_LEFT:
            turnLeftSide(MotorDir_t::RELEASE);
            turnRightSide(MotorDir_t::FORWARD);
            break;
        FORWARD_RIGHT:
            turnLeftSide(MotorDir_t::FORWARD);
            turnRightSide(MotorDir_t::RELEASE);
            break;
        REVERSE:
            turnLeftSide(MotorDir_t::REVERSE);
            turnRightSide(MotorDir_t::REVERSE);
            break;
        REVERSE_LEFT:
            turnLeftSide(MotorDir_t::RELEASE);
            turnRightSide(MotorDir_t::REVERSE);
            break;
        REVERSE_RIGHT:
            turnLeftSide(MotorDir_t::REVERSE);
            turnRightSide(MotorDir_t::REVERSE);
            break;
        LEFT:
            turnLeftSide(MotorDir_t::REVERSE);
            turnRightSide(MotorDir_t::FORWARD);
            break;
        RIGHT:
            turnLeftSide(MotorDir_t::FORWARD);
            turnRightSide(MotorDir_t::REVERSE);
            break;
        default:
            std::cerr << "moveCar() Invalid car direction:" 
                      << static_cast<uint8_t>(carDir) << std::endl;
            return;
    }
}

}