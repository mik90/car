#include "pwmWheel.hpp"

namespace Car
{
PwmWheel::PwmWheel(wPiPins::pin_t pin) :
                                       m_pwmPin{pin},
                                       m_time{std::chrono::microseconds{0}, false}
{
    // Default speed is medium
    m_pulseLength = PWM::Speed::Medium;

    // These values are kind of arbitrary, they're pulled from
    // the UCTRONICS codebase and have no explanation
    switch(pin)
    {
        case wPiPins::PwmWheelRR:
            m_motorForward = 0b00001000;  // Bit 3
            m_motorReverse = 0b00000100;  // Bit 2
            break;
        case wPiPins::PwmWheelRl:
            m_motorForward = 0b00010000;  // Bit 4
            m_motorReverse = 0b00000010;  // Bit 1
            break;
        case wPiPins::PwmWheelFR:
            m_motorForward = 0b00100000;  // Bit 5
            m_motorReverse = 0b10000000;  // Bit 7
            break;
        case wPiPins::PwmWheelFL:
            m_motorForward = 0b00000001;  // Bit 0
            m_motorReverse = 0b01000000;  // Bit 6
            break;
        default:
            std::cerr << "PwmWheel() invalid pin\n";
            m_motorForward = 0b00000000;
            m_motorReverse = 0b00000000;
            break;
    }
}

// Command for turning the motor forward, reverse, or releasing,
// does not adjust speed
[[nodiscard]]
std::bitset<8> PwmWheel::calcMotorDirCommand(MotorDir_t motorDir)
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
void PwmWheel::setSpeed(PWM::pulseLength pulseLen)
{
    m_pulseLength = pulseLen;
}

// Adjust speed, does not turn the motor itself
void PwmWheel::outputPwmCommand()
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
            // We are within the pulse range, write high
            digitalWrite(m_pwmPin, HIGH);
        else
            // We are past the pulse range, write low 
            digitalWrite(m_pwmPin, LOW);
    }
    else
    {
        // We've passed the full period, will need to start the timestamp
        // next time around
        m_time.inPulsePeriod = false;
    }
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


} // end of namespace