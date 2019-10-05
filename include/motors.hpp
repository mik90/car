#ifndef MOTORS_HPP_
#define MOTORS_HPP_

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

        // Same as 10 milliseconds
        constexpr std::chrono::microseconds fullPwmPeriod{10000};
        
        constexpr std::chrono::microseconds halfPwmPeriod{fullPwmPeriod / 2};
    }

    struct pwmTimestamp
    {
        std::chrono::microseconds tStart{0};
        bool inPulsePeriod = false;
    };

    class PwmMotor
    {
        public:
            PwmMotor(pin_t const pin);
            void turnMotor(MotorDir_t motorDir);
            void writeToMotorRegister(std::bitset<8> registerData);
            void pwmWrite(PWM::pulseLength pulseLen);
        private:
            pin_t           m_gpioPin;
            std::bitset<32> m_motorBitset;
            std::bitset<8>  m_motorRegisterA;
            std::bitset<8>  m_motorRegisterB;
            pwmTimestamp    m_time;
    };

    /** @brief All of the functions and data types relating to motor usage **/
    class Motors
    {
        public:
            Motors();
            void turnLeftSide (MotorDir_t motorDir, PWM::pulseLength pLength);
            void turnRightSide(MotorDir_t motorDir, PWM::pulseLength pLength);

        private:
            PwmMotor m_RearRight {wPiPins::MotorPwmRR};
            PwmMotor m_RearLeft  {wPiPins::MotorPwmRL};
            PwmMotor m_FrontRight{wPiPins::MotorPwmFR};
            PwmMotor m_FrontLeft {wPiPins::MotorPwmFL};
    };

}

#endif