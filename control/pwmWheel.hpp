#ifndef PWM_WHEEL_HPP_
#define PWM_WHEEL_HPP_

#include <memory>
#include <bitset>
#include <chrono>

#include <cerrno>
#include <cmath>
#include <cstdint>

#include "commonRpi.hpp"

namespace Car
{

namespace PWM
{
    using namespace std::chrono_literals;
    using pulseLength = std::chrono::microseconds;

    // 1000 microseconds is the same as 10 milliseconds
    constexpr std::chrono::microseconds fullPwmPeriod{10ms};
    
    namespace Speed
    {
        // Full cycle
        constexpr PWM::pulseLength Fast{fullPwmPeriod};
        // Half cycle
        constexpr PWM::pulseLength Medium{fullPwmPeriod / 2};
        // Quarter cycle
        constexpr PWM::pulseLength Slow{fullPwmPeriod / 4};
    }
}

enum class MotorDir_t {FORWARD, REVERSE, RELEASE};
// Printing out MotorDir_t
std::ostream& operator<<(std::ostream& out, MotorDir_t dir);

struct pwmTimestamp
{
    std::chrono::microseconds tStart{0};
    bool inPulsePeriod{false};
};

class PwmWheel
{
    public:
        PwmWheel(UCTronicsPins::pin_t pwmPin);
        std::bitset<8> calcMotorDirCommand(MotorDir_t motorDir);
        void setSpeed(PWM::pulseLength pulseLen);
        void outputPwmCommand();
    private:
        UCTronicsPins::pin_t m_pwmPin;
        PWM::pulseLength m_pulseLength;
        std::bitset<32>  m_pwmIdentityBitset;
        std::bitset<8>   m_motorForward;
        std::bitset<8>   m_motorReverse;
        pwmTimestamp     m_time;
};

}

#endif