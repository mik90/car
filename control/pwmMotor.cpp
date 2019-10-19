#include "effectors.hpp"

namespace Car
{

// Command for turning the motor forward, reverse, or releasing,
// does not adjust speed
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

} // end of namespace