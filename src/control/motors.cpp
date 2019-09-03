#include <iostream>
#include <cstddef>

#include "wiringPi.h"

#include "common_rpi.hpp"
#include "motors.hpp"

namespace Car
{


void Motors::writeToMotorRegister(uint8_t registerData)
{
    digitalWrite(BCM::MOTOR_LATCH, LOW);
    digitalWrite(BCM::MOTOR_DATA, LOW);

    // bitmask is a bit that shifts right every iteration.
    // starts at the leftmost bit and ends at the rightmost bit
    for (uint8_t bitmask = 0b1000000; bitmask > 0b00000000; bitmask >>= 1)
    {
        delayMicroseconds(1);

        digitalWrite(BCM::MOTOR_CLK, LOW);

        // We can only tell one motor to turn at a time via the
        // MOTOR_DATA signal.

        // At this index of the bitmask, check if we're supposed to
        // make this motor turn (registerData will have that bit as 1 if so)
        if (registerData & bitmask)
        {
            // Both bits are 1 so this motor needs to turn
            digitalWrite(BCM::MOTOR_DATA, HIGH);
        }
        else
        {
            // Either the motor is not supposed to turn or it's not the motor's
            // turn to write to MOTOR_DATA
            digitalWrite(BCM::MOTOR_DATA, LOW);
        }

        delayMicroseconds(1);
        digitalWrite(BCM::MOTOR_CLK, HIGH);
    }
        
    digitalWrite(BCM::MOTOR_LATCH, HIGH);
}

void Motors::turnWheelMotor(bcm_pin_t motorPin, MotorDir_t motorDir)
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
    switch (motorDir)
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
            std::cerr << "turnMotor() invalid motor direction:"
                      << static_cast<uint8_t>(motorDir) << std::endl;
            return;
    }

    writeToMotorRegister(register_data);
}


void Motors::moveCar(Direction_t carDir)
{
    auto turnLeftSide = [this](MotorDir_t motorDir)
    {
        turnWheelMotor(BCM::MotorPWM_FL, motorDir);
        turnWheelMotor(BCM::MotorPWM_RL, motorDir);
    };
    auto turnRightSide = [this](MotorDir_t motorDir)
    {
        turnWheelMotor(BCM::MotorPWM_FR, motorDir);
        turnWheelMotor(BCM::MotorPWM_RR, motorDir);
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

} // End of namespace