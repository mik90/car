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
        // These are some hardcoded values from the old code. They are
        // bitstrings where only one index is 1 and the rest are 0s

        // 5th index in the bitstring 1. 2^5 = 32
        constexpr std::bitset<32> bitMotorPwmRR{1 << wPiPins::MotorPwmRR};
        // 6th index in the bitstring is 1. 2^6 = 64
        constexpr std::bitset<32> bitMotorPwmRL{1 << wPiPins::MotorPwmRL};
        // 13th index in the bitstring is 1. 2^13 = 8192
        constexpr std::bitset<32> bitMotorPwmFR{1 << wPiPins::MotorPwmFR};
        // 19th index in the bitstring is 1. 2^19 = 524,288
        constexpr std::bitset<32> bitMotorPwmFL{1 << wPiPins::MotorPwmFL};

        // Same as 10 milliseconds
        constexpr std::chrono::microseconds pwmPeriod{10000};
        
        constexpr std::chrono::microseconds halfPwmPeriod{pwmPeriod / 2};
    }

    /** @brief All of the functions and data types relating to motor usage **/
    class Motors
    {
        private:
            std::shared_ptr<volatile uint32_t> m_gpioMmap;
            
            // Init motor and servo pins
            void initPanTilt();
            void initDcMotorController();

            // Control motors
            void writeToMotorRegister(uint8_t registerData);
        public:
            Motors();
            ~Motors();

            void setMemoryMap(const std::shared_ptr<volatile uint32_t>& gpioMmap_ptr);
            void turnDcMotor(pin_t motorPin, MotorDir_t motorDir);
    };

}

#endif