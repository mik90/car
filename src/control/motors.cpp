#include <iostream>
#include <cstddef>

#include "wiringPi.h"

#include "common_rpi.hpp"
#include "motors.hpp"

namespace Car
{


void Motors::writeToMotorRegister(uint8_t latch_data)
{
    digitalWrite(BCM::MOTOR_LATCH, LOW);
    digitalWrite(BCM::MOTOR_DATA, LOW);

    for (int i = 0; i < 8; i++)
    {
        delayMicroseconds(1);

        digitalWrite(BCM::MOTOR_CLK, LOW);

        if (latch_data & (1 << (7 - i)))
        {
            digitalWrite(BCM::MOTOR_DATA, HIGH);
        }
        else
        {
            digitalWrite(BCM::MOTOR_DATA, LOW);
        }

        delayMicroseconds(1);
        digitalWrite(BCM::MOTOR_CLK, HIGH);
    }
        
    digitalWrite(BCM::MOTOR_LATCH, HIGH);
}

void Motors::turnWheelMotor(bcm_pin_t motorPin, MotorDir_t dir)
{
    // The motor has two inputs, each motor will correspond to
    // to two bits in the motor register.
    uint8_t outputA  = 0;
    uint8_t outputB  = 0;

    switch (motorPin)
    {
        case BCM::MotorPWM_RR:
            outputA = static_cast<uint8_t>(MotorBit_t::MOTOR_RR_A);
            outputB = static_cast<uint8_t>(MotorBit_t::MOTOR_RR_B);
            break;
        case BCM::MotorPWM_RL:
            outputA = static_cast<uint8_t>(MotorBit_t::MOTOR_RL_A);
            outputB = static_cast<uint8_t>(MotorBit_t::MOTOR_RL_B);
            break;
        case BCM::MotorPWM_FR:
            outputA = static_cast<uint8_t>(MotorBit_t::MOTOR_FR_A);
            outputB = static_cast<uint8_t>(MotorBit_t::MOTOR_FR_B);
            break;
        case BCM::MotorPWM_FL:
            outputA = static_cast<uint8_t>(MotorBit_t::MOTOR_FL_A);
            outputB = static_cast<uint8_t>(MotorBit_t::MOTOR_FL_B);
            break;
        default:
            std::cerr << "turnMotor() invalid motor pin:"
                      << motorPin << std::endl;
            return;
    }

    uint8_t register_data = 0;
    switch (dir)
    {
        case MotorDir_t::FORWARD:
            register_data |= outputA;
            register_data &= ~outputB;
            break;
        case MotorDir_t::REVERSE:
            register_data &= ~outputA;
            register_data |= outputB;
            break;
        case MotorDir_t::RELEASE:
            register_data &= ~outputA;
            register_data &= ~outputB;
            break;
        default:
            std::cerr << "turnMotor() invalid direction:"
                      << static_cast<uint8_t>(dir) << std::endl;
            return;
    }

    writeToMotorRegister(register_data);
}


void Motors::moveCar(Direction_t dir)
{
    auto turnLeftSide = [this](MotorDir_t dir)
    {
        turnWheelMotor(BCM::MotorPWM_FL, dir);
        turnWheelMotor(BCM::MotorPWM_RL, dir);
    };
    auto turnRightSide = [this](MotorDir_t dir)
    {
        turnWheelMotor(BCM::MotorPWM_FR, dir);
        turnWheelMotor(BCM::MotorPWM_RR, dir);
    };

    switch(dir)
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
            std::cerr << "moveCar() Invalid dir:" <<
            static_cast<uint8_t>(dir) << std::endl;
            return;
    }
}

} // End of namespace