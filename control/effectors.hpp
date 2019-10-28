#ifndef EFFECTORS_HPP_
#define EFFECTORS_HPP_

#include <memory>
#include <bitset>
#include <chrono>

#include <cerrno>
#include <cmath>
#include <cstdint>

#include "common_rpi.hpp"

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
// Print out Motor directions
std::ostream& operator<<(std::ostream& out, MotorDir_t dir);

struct pwmTimestamp
{
    std::chrono::microseconds tStart{0};
    bool inPulsePeriod = false;
};

class PwmMotor
{
    public:
        PwmMotor(UCTronicsPins::pin_t const pin);
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

/** @brief API for controlling motors and other environment-affecting periphials **/
class Effectors
{
    public:
        Effectors();
        void turnWheels(MotorDir_t leftSide, MotorDir_t rightSide,
                        PWM::pulseLength pLength);
        void turnWheels(MotorDir_t leftSide, MotorDir_t rightSide);
        void beep(std::chrono::seconds duration);
    private:
        void outputWheelCommands(std::bitset<8> motorCommands);
        PwmMotor m_RearRight {UCTronicsPins::PwmMotorRR};
        PwmMotor m_RearLeft  {UCTronicsPins::PwmMotorRl};
        PwmMotor m_FrontRight{UCTronicsPins::PwmMotorFR};
        PwmMotor m_FrontLeft {UCTronicsPins::PwmMotorFL};
};

}

#endif