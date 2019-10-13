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

        // These are some hardcoded values from the old code. They are
        // bitstrings where only one index is 1 and the rest are 0s

        // 5th index in the bitstring 1. 2^5 = 32
        constexpr std::bitset<32> bitRR{1 << wPiPins::MotorPwmRR};
        // 6th index in the bitstring is 1. 2^6 = 64
        constexpr std::bitset<32> bitRL{1 << wPiPins::MotorPwmRL};
        // 13th index in the bitstring is 1. 2^13 = 8192
        constexpr std::bitset<32> bitFR{1 << wPiPins::MotorPwmFR};
        // 19th index in the bitstring is 1. 2^19 = 524,288
        constexpr std::bitset<32> bitFL{1 << wPiPins::MotorPwmFL};

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
            PwmMotor(pin_t const pin);
            std::bitset<8> calcMotorDirCommand(MotorDir_t motorDir);
            void setSpeed(PWM::pulseLength pulseLen);
            void outputPwmCommand();
        private:
            pin_t            m_pwmPin;
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
            void turnMotor(PwmMotor& motor, MotorDir_t dir);
            void turnMotors(MotorDir_t leftSide, MotorDir_t rightSide,
                            PWM::pulseLength pLength);
            void turnMotors(MotorDir_t leftSide, MotorDir_t rightSide);
            void beep(std::chrono::seconds duration);
            void outputMotorCommands(std::bitset<8> motorCommands);
            PwmMotor m_RearRight {wPiPins::MotorPwmRR};
            PwmMotor m_RearLeft  {wPiPins::MotorPwmRL};
            PwmMotor m_FrontRight{wPiPins::MotorPwmFR};
            PwmMotor m_FrontLeft {wPiPins::MotorPwmFL};
        private:
            std::bitset<8> m_motorCommand;
    };

}

#endif