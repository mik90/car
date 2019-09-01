#ifndef CAR_HPP_
#define CAR_HPP_

#include <cstdint>
#include <memory>

#include "sensors.hpp"
#include "motors.hpp"

namespace Car
{
    class Car
    {
        private:
            std::shared_ptr<volatile uint32_t> m_gpio_mmap;
            Sensors m_sensors;
            Motors  m_motors;
        public:
            Car();
            ~Car();
            void initBeep();

    };

}


#endif