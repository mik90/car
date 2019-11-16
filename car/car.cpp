#include <iostream>
#include <thread>

#include "wiringPi.h"

#include "irRemote.hpp"

#include "car.hpp"

namespace Car
{

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

    m_motors.turnWheels(leftSide, rightSide);
}


void Car::parseIrCommand(uint16_t irCommand)
{

    switch(irCommand)
    {
        case IrRemoteCommands::GoForward:
            std::cout << "DEBUG IR Remote Forward" << std::endl;
            moveCar(CarMovement_t::FORWARD);
            break;
        case IrRemoteCommands::GoBackward:
            std::cout << "DEBUG IR Remote Reverse" << std::endl;
            moveCar(CarMovement_t::REVERSE);
            break;
        case IrRemoteCommands::TurnLeft:
            std::cout << "DEBUG IR Remote Left" << std::endl;
            moveCar(CarMovement_t::LEFT);
            break;
        case IrRemoteCommands::TurnRight:
            std::cout << "DEBUG IR Remote Right" << std::endl;
            moveCar(CarMovement_t::RIGHT);
            break;
        case IrRemoteCommands::Stop:
            std::cout << "DEBUG IR Remote Stop" << std::endl;
            moveCar(CarMovement_t::STOP);
            break;

        case IrRemoteCommands::IncreaseWheelSpeed:
            std::cout << "DEBUG IR More speed" << std::endl;
            break;
        case IrRemoteCommands::DecreaseWheelSpeed:
            std::cout << "DEBUG IR less speed" << std::endl;
            break;

        case IrRemoteCommands::TiltUp:
            std::cout << "DEBUG IR tilt up" << std::endl;
            break;
        case IrRemoteCommands::TiltDown:
            std::cout << "DEBUG IR tilt down" << std::endl;
            break;

        case IrRemoteCommands::PanRight:
            std::cout << "DEBUG IR pan right" << std::endl;
            break;
        case IrRemoteCommands::PanLeft:
            std::cout << "DEBUG IR pan left" << std::endl;
            break;

        default:
            std::cout << "DEBUG IR default:" << irCommand << std::endl;
            break;
    }
}


void Car::beep(std::chrono::seconds duration)
{
    digitalWrite(wPiPins::Beep, HIGH);
    std::this_thread::sleep_for(duration);
    digitalWrite(wPiPins::Beep, LOW);
}
void Car::pan(dutyCycle angle)  { m_motors.pan(angle);  }
void Car::tilt(dutyCycle angle) { m_motors.tilt(angle); }

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