#include <iostream>
#include <thread>

#include "wiringPi.h"

#include "irRemote.hpp"

#include "car.hpp"

namespace Car
{

void Car::moveCar(CarMovement_t carDir)
{

    switch(carDir)
    {
        case CarMovement_t::FORWARD:
            m_motors.turnWheels(MotorDir_t::FORWARD, MotorDir_t::FORWARD);
            break;
        case CarMovement_t::FORWARD_LEFT:
            m_motors.turnWheels(MotorDir_t::RELEASE, MotorDir_t::FORWARD);
            break;
        case CarMovement_t::FORWARD_RIGHT:
            m_motors.turnWheels(MotorDir_t::FORWARD, MotorDir_t::RELEASE);
            break;
        case CarMovement_t::REVERSE:
            m_motors.turnWheels(MotorDir_t::REVERSE, MotorDir_t::REVERSE);
            break;
        case CarMovement_t::REVERSE_LEFT:
            m_motors.turnWheels(MotorDir_t::RELEASE, MotorDir_t::REVERSE);
            break;
        case CarMovement_t::REVERSE_RIGHT:
            m_motors.turnWheels(MotorDir_t::REVERSE, MotorDir_t::RELEASE);
            break;
        case CarMovement_t::LEFT:
            m_motors.turnWheels(MotorDir_t::REVERSE, MotorDir_t::FORWARD);
            break;
        case CarMovement_t::RIGHT:
            m_motors.turnWheels(MotorDir_t::FORWARD, MotorDir_t::REVERSE);
            break;
        case CarMovement_t::STOP:
            m_motors.turnWheels(MotorDir_t::RELEASE, MotorDir_t::RELEASE);
            break;
        default:
            std::cerr << "moveCar() Invalid car direction:" 
                      << carDir << std::endl;
            return;
    }
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
void Car::panLeft()  { m_motors.panLeft();  }
void Car::panRight() { m_motors.panRight(); }

void Car::tiltDown() { m_motors.tiltDown(); }
void Car::tiltUp()   { m_motors.tiltUp();   }

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