#include <iostream>
#include <thread>
#include <chrono>

#include "car.hpp"
#include "wiringPi.h"

namespace Car
{

void Car::beepSeconds(unsigned int sec)
{
    digitalWrite(wPiPins::Beep, HIGH);
    std::this_thread::sleep_for(std::chrono::seconds(sec));
    digitalWrite(wPiPins::Beep, LOW);
}


void Car::moveCar(CarDirection_t carDir)
{
    auto turnLeftSide = [this](MotorDir_t motorDir)
    {
        m_motors.turnDcMotor(wPiPins::MotorPwmFL, motorDir);
        m_motors.turnDcMotor(wPiPins::MotorPwmRL, motorDir);
    };
    auto turnRightSide = [this](MotorDir_t motorDir)
    {
        m_motors.turnDcMotor(wPiPins::MotorPwmFR, motorDir);
        m_motors.turnDcMotor(wPiPins::MotorPwmRR, motorDir);
    };

    switch(carDir)
    {
        case CarDirection_t::FORWARD:
            turnLeftSide(MotorDir_t::FORWARD);
            turnRightSide(MotorDir_t::FORWARD);
            break;
        case CarDirection_t::FORWARD_LEFT:
            turnLeftSide(MotorDir_t::RELEASE);
            turnRightSide(MotorDir_t::FORWARD);
            break;
        case CarDirection_t::FORWARD_RIGHT:
            turnLeftSide(MotorDir_t::FORWARD);
            turnRightSide(MotorDir_t::RELEASE);
            break;
        case CarDirection_t::REVERSE:
            turnLeftSide(MotorDir_t::REVERSE);
            turnRightSide(MotorDir_t::REVERSE);
            break;
        case CarDirection_t::REVERSE_LEFT:
            turnLeftSide(MotorDir_t::RELEASE);
            turnRightSide(MotorDir_t::REVERSE);
            break;
        case CarDirection_t::REVERSE_RIGHT:
            turnLeftSide(MotorDir_t::REVERSE);
            turnRightSide(MotorDir_t::REVERSE);
            break;
        case CarDirection_t::LEFT:
            turnLeftSide(MotorDir_t::REVERSE);
            turnRightSide(MotorDir_t::FORWARD);
            break;
        case CarDirection_t::RIGHT:
            turnLeftSide(MotorDir_t::FORWARD);
            turnRightSide(MotorDir_t::REVERSE);
            break;
        default:
            std::cerr << "moveCar() Invalid car direction:" 
                      << carDir << std::endl;
            return;
    }
}

std::ostream& operator<<(std::ostream& out, CarDirection_t dir)
{
    switch(dir)
    {
        case CarDirection_t::FORWARD: out << "FORWARD"; break;
        case CarDirection_t::FORWARD_LEFT: out << "FORWARD_LEFT"; break;
        case CarDirection_t::FORWARD_RIGHT: out << "FORWARD_RIGHT"; break;
        case CarDirection_t::REVERSE: out << "REVERSE"; break;
        case CarDirection_t::REVERSE_LEFT: out << "REVERSE_LEFT"; break;
        case CarDirection_t::REVERSE_RIGHT: out << "REVERSE_RIGHT"; break;
        case CarDirection_t::LEFT: out << "LEFT"; break;
        case CarDirection_t::RIGHT: out << "RIGHT"; break;
        default: out.setstate(std::ios_base::failbit);
    }
    return out;
}



} // End of namespace Car