#include <iostream>
#include <bitset>
#include <thread>

#include <cstddef>

#include "wiringPi.h"

#include "effectors.hpp"

namespace Car
{


void Effectors::turnMotor(PwmMotor& motor, MotorDir_t dir)
{
    motor.outputPwmCommand();
    this->outputMotorCommands(motor.calcMotorDirCommand(dir));
}

// Effectors Implementation ---------------------------------------

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
    // Logical OR all of the motors together
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

// PwmMotor Implementation ---------------------------------------

// Command for turning the motor, does not adjust speed
[[nodiscard]]
std::bitset<8> PwmMotor::calcMotorDirCommand(MotorDir_t motorDir)
{
    std::bitset<8> motorCommand{0b00000000};
    switch (motorDir)
    {
        case MotorDir_t::FORWARD:
            motorCommand |= m_motorForward;
            motorCommand &= ~m_motorReverse;
            break;
        case MotorDir_t::REVERSE:
            motorCommand &= ~m_motorForward;
            motorCommand |= m_motorReverse;
            break;
        case MotorDir_t::RELEASE:
            motorCommand &= ~m_motorForward;
            motorCommand &= ~m_motorReverse;
            break;
        default:
            std::cerr << "calcMotorCommand() invalid motor direction:"
                      << motorDir << std::endl;
            break;
    }

    return motorCommand;
}

// Update the speed
void PwmMotor::setSpeed(PWM::pulseLength pulseLen)
{
    m_pulseLength = pulseLen;
}

// Adjust speed, does not turn the motor itself
void PwmMotor::outputPwmCommand()
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
        if (tDelta <= m_pulseLength)
            // We are within the pulse range, set the bits
            RpiInterface::writeGpioBits(m_pwmIdentityBitset);
        else
            // We are past the pulse range, clear the bits
            RpiInterface::clearGpioBits(m_pwmIdentityBitset);
    }
    else
    {
        // We've passed the full period, will need to start the timestamp
        // next time around
        m_time.inPulsePeriod = false;
    }
}


// Beeper/buzzer implementation -----------------------------------------
void Effectors::beep(std::chrono::seconds duration)
{
    digitalWrite(wPiPins::Beep, HIGH);
    std::this_thread::sleep_for(duration);
    digitalWrite(wPiPins::Beep, LOW);
}

// Debugging support
std::ostream& operator<<(std::ostream& out, MotorDir_t dir)
{
    switch(dir)
    {
        case MotorDir_t::FORWARD: out << "FORWARD"; break;
        case MotorDir_t::REVERSE: out << "REVERSE"; break;
        case MotorDir_t::RELEASE: out << "RELEASE"; break;
        default: out.setstate(std::ios_base::failbit);
    }
    return out;
}

} // End of namespace