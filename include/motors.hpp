#ifndef MOTORS_HPP_
#define MOTORS_HPP_

#include <memory>
#include <cerrno>
#include <cstdint>

#include "common_rpi.hpp"

// Note: Old code has DCMotorInit(), is this needed?;


namespace Car
{
    /** @brief All of the functions and data types relating to motor usage **/
    class Motors
    {
        private:
            std::shared_ptr<volatile uint32_t> m_gpioMmap;
            
            // Init motor and servo pins
            void initPanTilt();
            void initPwm();
            void initDcMotorController();

            // Control motors
            void writeToMotorRegister(uint8_t registerData);
        public:
            Motors();

            void setMemoryMap(const std::shared_ptr<volatile uint32_t>& gpioMmap_ptr);
            void turnDcMotor(bcm_pin_t motorPin, MotorDir_t motorDir);
    };

}

#endif