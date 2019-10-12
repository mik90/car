#include <iostream>
#include <bitset>
#include <thread>

#include <cstddef>

#include "wiringPi.h"

#include "effectors.hpp"

namespace Car
{

// Effectors Implementation ---------------------------------------

void Effectors::turnLeftSide (MotorDir_t motorDir)
{
    m_RearLeft.turnMotor(motorDir);
    m_FrontLeft.turnMotor(motorDir);
}

void Effectors::turnLeftSide (MotorDir_t motorDir, PWM::pulseLength pLength)
{
    m_RearLeft.setSpeed(pLength);
    m_RearLeft.turnMotor(motorDir);

    m_FrontLeft.setSpeed(pLength);
    m_FrontLeft.turnMotor(motorDir);
}

void Effectors::turnRightSide(MotorDir_t motorDir)
{
    m_RearRight.turnMotor(motorDir);
    m_FrontRight.turnMotor(motorDir);
}

void Effectors::turnRightSide(MotorDir_t motorDir, PWM::pulseLength pLength)
{
    m_RearRight.setSpeed(pLength);
    m_RearRight.turnMotor(motorDir);
    
    m_FrontRight.setSpeed(pLength);
    m_FrontRight.turnMotor(motorDir);
}

// PWM Implementation ---------------------------------------

void PwmMotor::turnMotor(MotorDir_t motorDir)
{
    std::bitset<8> registerData{0b00000000};
    switch (motorDir)
    {
        case MotorDir_t::FORWARD:
            registerData |= m_motorRegisterA;
            registerData &= ~m_motorRegisterB;
            break;
        case MotorDir_t::REVERSE:
            registerData &= ~m_motorRegisterA;
            registerData |= m_motorRegisterB;
            break;
        case MotorDir_t::RELEASE:
            registerData &= ~m_motorRegisterA;
            registerData &= ~m_motorRegisterB;
            break;
        default:
            std::cerr << "turnMotor() invalid motor direction:"
                      << static_cast<uint8_t>(motorDir) << std::endl;
            return;
    }
    writeToMotorRegister(registerData);
}

void PwmMotor::setSpeed(PWM::pulseLength pulseLen)
{
    using namespace std::chrono;

    if (!m_time.inPulsePeriod)
    {
        // Start new pulse period
        m_time.inPulsePeriod = true;
        m_time.tStart = static_cast<microseconds>(micros());
    }

    // Timestamp management, check prev time vs now
    microseconds tNow = static_cast<microseconds>(micros());
    microseconds tDelta = tNow - m_time.tStart;

    /**
     *
     * min      half     full
     *  |        |        |
     *  |<------set------>|<clear>
     * Set value if within the range, clear it if not, if we're above
     * the full period, we'll redo the timestamp for the next cycle
     **/
    if (tDelta < PWM::fullPwmPeriod)
    {
        if (tDelta <= pulseLen)
            // We are within the pulse range, set the bits
            RpiInterface::writeGpioBits(m_motorBitset);
        else
            // We are past the pulse range, clear the bits
            RpiInterface::clearGpioBits(m_motorBitset);
    }
    else
    {
        // We've passed the full period, will need to start the timestamp
        // next time around
        m_time.inPulsePeriod = false;
    }
}

void PwmMotor::writeToMotorRegister(std::bitset<8> registerData)
{
    digitalWrite(wPiPins::MotorLatch, LOW);
    digitalWrite(wPiPins::MotorData, LOW);

    std::cout << "Writing to motor register...\n";
    std::cout << "Register data:" << registerData << std::endl;

    // 'bitmask' is a bit that shifts right every iteration.
    // starts at the leftmost (7th) bit and ends at the rightmost (0th) bit
    for (std::bitset<8> bitmask{0b10000000}; !bitmask.none(); bitmask >>= 1)
    {
        delayMicroseconds(1);

        digitalWrite(wPiPins::MotorClock, LOW);

        // We can only tell one motor to turn at a time via the
        // MotorData signal.

        // Check if any of the bits are 1
        if ((registerData & bitmask).any())
        {
            // Both bits are 1 so this motor needs to turn
            std::cout << "Writing high. bitmask:" << std::bitset<8>(bitmask) << std::endl;
            digitalWrite(wPiPins::MotorData, HIGH);
        }
        else
        {
            // Either the motor is not supposed to turn or it's not the motor's
            // turn to write to MotorData
            std::cout << "Writing low. bitmask:" << std::bitset<8>(bitmask) << std::endl;
            digitalWrite(wPiPins::MotorData, LOW);
        }

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


} // End of namespace