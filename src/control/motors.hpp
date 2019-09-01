#ifndef MOTORS_HPP_
#define MOTORS_HPP_

#include <iostream>
#include <cstdlib>
#include <memory>
#include <cerrno>
#include <cstdint>

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <wiringPi.h>
#include <softPwm.h>

#include "constants.hpp"
#include "ws2811.h"

namespace Car
{
    using bcm_pin_t = unsigned int;

    /** @brief All of the functions and data types relating to motor usage **/
    class Motors
    {
        private:
            std::shared_ptr<volatile uint32_t> m_gpio_mmap;
        public:
            explicit Motors(std::shared_ptr<volatile uint32_t> gpio_ptr);
            ~Motors();

            void initPanTilt();
            void initPWM();
            void initMotorControll();
            void writeToShiftRegister(uint8_t latch_data)
    };

}

#endif