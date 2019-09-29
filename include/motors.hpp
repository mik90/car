#ifndef MOTORS_HPP_
#define MOTORS_HPP_

#include <memory>
#include <cerrno>
#include <cstdint>

#include "common_rpi.hpp"

namespace Car
{
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