#include <iostream>
#include <thread>

#include "wiringPi.h"

#include "car.hpp"

namespace Car
{

enum class MotorDir_t {FORWARD, REVERSE, RELEASE};

void Car::moveCar(CarMovement_t carDir)
{
    MotorDir_t leftSide, rightSide;

    switch(carDir)
    {
        case CarMovement_t::FORWARD:
            leftSide  = MotorDir_t::FORWARD;
            rightSide = MotorDir_t::FORWARD;
            break;
        case CarMovement_t::FORWARD_LEFT:
            leftSide  = MotorDir_t::RELEASE;
            rightSide = MotorDir_t::FORWARD;
            break;
        case CarMovement_t::FORWARD_RIGHT:
            leftSide  = MotorDir_t::FORWARD;
            rightSide = MotorDir_t::RELEASE;
            break;
        case CarMovement_t::REVERSE:
            leftSide  = MotorDir_t::REVERSE;
            rightSide = MotorDir_t::REVERSE;
            break;
        case CarMovement_t::REVERSE_LEFT:
            leftSide  = MotorDir_t::RELEASE;
            rightSide = MotorDir_t::REVERSE;
            break;
        case CarMovement_t::REVERSE_RIGHT:
            leftSide  = MotorDir_t::REVERSE;
            rightSide = MotorDir_t::RELEASE;
            break;
        case CarMovement_t::LEFT:
            leftSide  = MotorDir_t::REVERSE;
            rightSide = MotorDir_t::FORWARD;
            break;
        case CarMovement_t::RIGHT:
            leftSide  = MotorDir_t::FORWARD;
            rightSide = MotorDir_t::REVERSE;
            break;
        case CarMovement_t::STOP:
            leftSide  = MotorDir_t::RELEASE;
            rightSide = MotorDir_t::RELEASE;
            break;
        default:
            std::cerr << "moveCar() Invalid car direction:" 
                      << carDir << std::endl;
            return;
    }

    // TODO
}

void Car::pan(int angle)
{
    // TODO
}
void Car::tilt(int angle)
{
    // TODO
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