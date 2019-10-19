#include <iostream>

#include "wiringPi.h"

#include "car.hpp"

namespace Car
{

void Car::moveCar(CarMovement_t carDir)
{

    switch(carDir)
    {
        case CarMovement_t::FORWARD:
            m_effectors.turnMotors(MotorDir_t::FORWARD, MotorDir_t::FORWARD);
            break;
        case CarMovement_t::FORWARD_LEFT:
            m_effectors.turnMotors(MotorDir_t::RELEASE, MotorDir_t::FORWARD);
            break;
        case CarMovement_t::FORWARD_RIGHT:
            m_effectors.turnMotors(MotorDir_t::FORWARD, MotorDir_t::RELEASE);
            break;
        case CarMovement_t::REVERSE:
            m_effectors.turnMotors(MotorDir_t::REVERSE, MotorDir_t::REVERSE);
            break;
        case CarMovement_t::REVERSE_LEFT:
            m_effectors.turnMotors(MotorDir_t::RELEASE, MotorDir_t::REVERSE);
            break;
        case CarMovement_t::REVERSE_RIGHT:
            m_effectors.turnMotors(MotorDir_t::REVERSE, MotorDir_t::RELEASE);
            break;
        case CarMovement_t::LEFT:
            m_effectors.turnMotors(MotorDir_t::REVERSE, MotorDir_t::FORWARD);
            break;
        case CarMovement_t::RIGHT:
            m_effectors.turnMotors(MotorDir_t::FORWARD, MotorDir_t::REVERSE);
            break;
        case CarMovement_t::STOP:
            m_effectors.turnMotors(MotorDir_t::RELEASE, MotorDir_t::RELEASE);
            break;
        default:
            std::cerr << "moveCar() Invalid car direction:" 
                      << carDir << std::endl;
            return;
    }
}

void Car::beep(std::chrono::seconds duration)
{
    m_effectors.beep(duration);
}

std::ostream& operator<<(std::ostream& out, CarMovement_t dir)
{
    switch(dir)
    {
        case CarMovement_t::FORWARD: out << "FORWARD"; break;
        case CarMovement_t::FORWARD_LEFT: out << "FORWARD_LEFT"; break;
        case CarMovement_t::FORWARD_RIGHT: out << "FORWARD_RIGHT"; break;
        case CarMovement_t::REVERSE: out << "REVERSE"; break;
        case CarMovement_t::REVERSE_LEFT: out << "REVERSE_LEFT"; break;
        case CarMovement_t::REVERSE_RIGHT: out << "REVERSE_RIGHT"; break;
        case CarMovement_t::LEFT: out << "LEFT"; break;
        case CarMovement_t::RIGHT: out << "RIGHT"; break;
        default:
            out << "Invalid CarMovement_t as uint_8:" << static_cast<uint8_t>(dir);
            out.setstate(std::ios_base::failbit);
            break;
    }
    return out;
}


} // End of namespace Car