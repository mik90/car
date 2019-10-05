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
    // Temporarily default the speed at 5000
    const PWM::pulseLength halfSpeed{5000};

    switch(carDir)
    {
        case CarDirection_t::FORWARD:
            m_motors.turnLeftSide(MotorDir_t::FORWARD, halfSpeed);
            m_motors.turnRightSide(MotorDir_t::FORWARD, halfSpeed);
            break;
        case CarDirection_t::FORWARD_LEFT:
            m_motors.turnLeftSide(MotorDir_t::RELEASE, halfSpeed);
            m_motors.turnRightSide(MotorDir_t::FORWARD, halfSpeed);
            break;
        case CarDirection_t::FORWARD_RIGHT:
            m_motors.turnLeftSide(MotorDir_t::FORWARD, halfSpeed);
            m_motors.turnRightSide(MotorDir_t::RELEASE, halfSpeed);
            break;
        case CarDirection_t::REVERSE:
            m_motors.turnLeftSide(MotorDir_t::REVERSE, halfSpeed);
            m_motors.turnRightSide(MotorDir_t::REVERSE, halfSpeed);
            break;
        case CarDirection_t::REVERSE_LEFT:
            m_motors.turnLeftSide(MotorDir_t::RELEASE, halfSpeed);
            m_motors.turnRightSide(MotorDir_t::REVERSE, halfSpeed);
            break;
        case CarDirection_t::REVERSE_RIGHT:
            m_motors.turnLeftSide(MotorDir_t::REVERSE, halfSpeed);
            m_motors.turnRightSide(MotorDir_t::REVERSE, halfSpeed);
            break;
        case CarDirection_t::LEFT:
            m_motors.turnLeftSide(MotorDir_t::REVERSE, halfSpeed);
            m_motors.turnRightSide(MotorDir_t::FORWARD, halfSpeed);
            break;
        case CarDirection_t::RIGHT:
            m_motors.turnLeftSide(MotorDir_t::FORWARD, halfSpeed);
            m_motors.turnRightSide(MotorDir_t::REVERSE, halfSpeed);
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