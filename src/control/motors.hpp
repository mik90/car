#ifndef MOTORS_HPP_
#define MOTORS_HPP_

#include <memory>
#include <cerrno>
#include <cstdint>

namespace Car
{
    /** @brief All of the functions and data types relating to motor usage **/
    class Motors
    {
        private:
            std::shared_ptr<volatile uint32_t> m_gpio_mmap;
        public:
            Motors();
            ~Motors();

            void setMemoryMap(std::shared_ptr<volatile uint32_t> gpio_ptr);
            void initPanTilt();
            void initPWM();
            void initMotorController();
            void writeToShiftRegister(uint8_t latch_data)
    };

}

#endif