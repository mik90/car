#include <iostream>

#include "wiringPi.h"

#include "car.hpp"

namespace Car
{

void Car::moveCar(CarDirection_t carDir)
{
    // Temporarily default the speed at 5000
    const PWM::pulseLength halfSpeed{5000};

    switch(carDir)
    {
        case CarDirection_t::FORWARD:
            m_effectors.turnLeftSide(MotorDir_t::FORWARD, halfSpeed);
            m_effectors.turnRightSide(MotorDir_t::FORWARD, halfSpeed);
            break;
        case CarDirection_t::FORWARD_LEFT:
            m_effectors.turnLeftSide(MotorDir_t::RELEASE, halfSpeed);
            m_effectors.turnRightSide(MotorDir_t::FORWARD, halfSpeed);
            break;
        case CarDirection_t::FORWARD_RIGHT:
            m_effectors.turnLeftSide(MotorDir_t::FORWARD, halfSpeed);
            m_effectors.turnRightSide(MotorDir_t::RELEASE, halfSpeed);
            break;
        case CarDirection_t::REVERSE:
            m_effectors.turnLeftSide(MotorDir_t::REVERSE, halfSpeed);
            m_effectors.turnRightSide(MotorDir_t::REVERSE, halfSpeed);
            break;
        case CarDirection_t::REVERSE_LEFT:
            m_effectors.turnLeftSide(MotorDir_t::RELEASE, halfSpeed);
            m_effectors.turnRightSide(MotorDir_t::REVERSE, halfSpeed);
            break;
        case CarDirection_t::REVERSE_RIGHT:
            m_effectors.turnLeftSide(MotorDir_t::REVERSE, halfSpeed);
            m_effectors.turnRightSide(MotorDir_t::REVERSE, halfSpeed);
            break;
        case CarDirection_t::LEFT:
            m_effectors.turnLeftSide(MotorDir_t::REVERSE, halfSpeed);
            m_effectors.turnRightSide(MotorDir_t::FORWARD, halfSpeed);
            break;
        case CarDirection_t::RIGHT:
            m_effectors.turnLeftSide(MotorDir_t::FORWARD, halfSpeed);
            m_effectors.turnRightSide(MotorDir_t::REVERSE, halfSpeed);
            break;
        case CarDirection_t::STOP:
            m_effectors.turnLeftSide(MotorDir_t::RELEASE);
            m_effectors.turnRightSide(MotorDir_t::RELEASE);
            break;
        default:
            std::cerr << "moveCar() Invalid car direction:" 
                      << carDir << std::endl;
            return;
    }
}

void Car::beepSeconds(std::chrono::seconds duration)
{
    m_effectors.beepSeconds(duration);
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