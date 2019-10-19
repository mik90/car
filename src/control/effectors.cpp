#include <iostream>
#include <bitset>
#include <thread>

#include <cstddef>

#include "wiringPi.h"

#include "effectors.hpp"

namespace Car
{

// Adjust speed of the motors before turning
void Effectors::turnMotors(MotorDir_t leftSide, MotorDir_t rightSide,
                           PWM::pulseLength pLength)
{
    m_RearLeft.setSpeed(pLength);
    m_FrontLeft.setSpeed(pLength);
    
    m_RearRight.setSpeed(pLength);
    m_FrontRight.setSpeed(pLength);

    turnMotors(leftSide, rightSide);
}

void Effectors::turnMotors(MotorDir_t leftSide, MotorDir_t rightSide)
{
    // Logical OR all of the commands together
    std::bitset<8> motorCommand{
                   m_RearLeft.calcMotorDirCommand(leftSide)
                 | m_FrontLeft.calcMotorDirCommand(leftSide)
                 | m_RearRight.calcMotorDirCommand(rightSide)
                 | m_FrontRight.calcMotorDirCommand(rightSide)};

    m_RearLeft.outputPwmCommand();
    m_FrontLeft.outputPwmCommand();
    m_RearRight.outputPwmCommand();
    m_FrontRight.outputPwmCommand();
    this->outputMotorCommands(motorCommand);
}

void Effectors::outputMotorCommands(std::bitset<8> motorCommands)
{
    digitalWrite(wPiPins::MotorLatch, LOW);
    digitalWrite(wPiPins::MotorData, LOW);

    std::cout << "Writing to motor register...\n";
    std::cout << "MotorCommands:" << motorCommands << std::endl;

    // 'bitmask' is a bit that shifts right every iteration.
    // starts at the leftmost (7th) bit and ends at the rightmost (0th) bit
    for (std::bitset<8> bitmask{0b10000000}; !bitmask.none(); bitmask >>= 1)
    {
        delayMicroseconds(1);

        digitalWrite(wPiPins::MotorClock, LOW);

        // We can only tell one motor to turn at a time via the
        // MotorData signal.

        // Write high if 1, write low if 0
        if ((motorCommands & bitmask).any())
            digitalWrite(wPiPins::MotorData, HIGH);
        else
            digitalWrite(wPiPins::MotorData, LOW);

        delayMicroseconds(1);
        digitalWrite(wPiPins::MotorClock, HIGH);
    }

    digitalWrite(wPiPins::MotorLatch, HIGH);
}

// Beeper/buzzer implementation -----------------------------------------
void Effectors::beep(std::chrono::seconds duration)
{
    digitalWrite(wPiPins::Beep, HIGH);
    std::this_thread::sleep_for(duration);
    digitalWrite(wPiPins::Beep, LOW);
}

// Debugging support ----------------------------------------------------
std::ostream& operator<<(std::ostream& out, MotorDir_t dir)
{
    switch(dir)
    {
        case MotorDir_t::FORWARD: out << "FORWARD"; break;
        case MotorDir_t::REVERSE: out << "REVERSE"; break;
        case MotorDir_t::RELEASE: out << "RELEASE"; break;
        default: 
            out << "Invalid MotorDir_t as uint_8:" << static_cast<uint8_t>(dir);
            out.setstate(std::ios_base::failbit);
            break;
    }
    return out;
}

} // End of namespace