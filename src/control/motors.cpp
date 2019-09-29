#include <iostream>
#include <bitset>

#include <cstddef>

#include "wiringPi.h"

#include "common_rpi.hpp"
#include "motors.hpp"

namespace Car
{


void Motors::writeToMotorRegister(uint8_t registerData)
{
    digitalWrite(wPiPins::MOTOR_LATCH, LOW);
    digitalWrite(wPiPins::MOTOR_DATA, LOW);

    std::cout << "Writing to motor register...\n";
    std::cout << "Register data:" << std::bitset<8>(registerData) << std::endl;
    // bitmask is a bit that shifts right every iteration.
    // starts at the leftmost bit and ends at the rightmost bit
    for (uint8_t bitmask = 0b1000000; bitmask > 0b00000000; bitmask >>= 1)
    {
        delayMicroseconds(1);

        digitalWrite(wPiPins::MOTOR_CLK, LOW);

        // We can only tell one motor to turn at a time via the
        // MOTOR_DATA signal.

        // At this index of the bitmask, check if we're supposed to
        // make this motor turn (registerData will have that bit as 1 if so)
        if (registerData & bitmask)
        {
            // Both bits are 1 so this motor needs to turn
            std::cout << "Writing high. bitmask:" << std::bitset<8>(bitmask) << std::endl;
            digitalWrite(wPiPins::MOTOR_DATA, HIGH);
        }
        else
        {
            // Either the motor is not supposed to turn or it's not the motor's
            // turn to write to MOTOR_DATA
            std::cout << "Writing low. bitmask:" << std::bitset<8>(bitmask) << std::endl;
            digitalWrite(wPiPins::MOTOR_DATA, LOW);
        }

        delayMicroseconds(1);
        digitalWrite(wPiPins::MOTOR_CLK, HIGH);
    }

    digitalWrite(wPiPins::MOTOR_LATCH, HIGH);
}

void Motors::turnDcMotor(pin_t motorPin, MotorDir_t motorDir)
{
    // The motor has two inputs, these values will both be put into
    // the motor register
    uint8_t outputA  = 0;
    uint8_t outputB  = 0;

    switch (motorPin)
    {
        case wPiPins::MotorPWM_RR:
            outputA = static_cast<uint8_t>(MotorBit_t::MOTOR_RR_A);
            outputB = static_cast<uint8_t>(MotorBit_t::MOTOR_RR_B);
            break;
        case wPiPins::MotorPWM_RL:
            outputA = static_cast<uint8_t>(MotorBit_t::MOTOR_RL_A);
            outputB = static_cast<uint8_t>(MotorBit_t::MOTOR_RL_B);
            break;
        case wPiPins::MotorPWM_FR:
            outputA = static_cast<uint8_t>(MotorBit_t::MOTOR_FR_A);
            outputB = static_cast<uint8_t>(MotorBit_t::MOTOR_FR_B);
            break;
        case wPiPins::MotorPWM_FL:
            outputA = static_cast<uint8_t>(MotorBit_t::MOTOR_FL_A);
            outputB = static_cast<uint8_t>(MotorBit_t::MOTOR_FL_B);
            break;
        default:
            std::cerr << "turnMotor() invalid motor pin:"
                      << motorPin << std::endl;
            return;
    }
    std::cout << "outputA:" << std::bitset<8>(outputA) << std::endl;
    std::cout << "outputB:" << std::bitset<8>(outputB) << std::endl;

    uint8_t registerData = 0;
    switch (motorDir)
    {
        case MotorDir_t::FORWARD:
            registerData |= outputA;
            registerData &= ~outputB;
            break;
        case MotorDir_t::REVERSE:
            registerData &= ~outputA;
            registerData |= outputB;
            break;
        case MotorDir_t::RELEASE:
            registerData &= ~outputA;
            registerData &= ~outputB;
            break;
        default:
            std::cerr << "turnMotor() invalid motor direction:"
                      << static_cast<uint8_t>(motorDir) << std::endl;
            return;
    }

    writeToMotorRegister(registerData);
}



} // End of namespace