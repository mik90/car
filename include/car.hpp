#ifndef CAR_HPP_
#define CAR_HPP_

#include <cstdint>
#include <memory>

#include "sensors.hpp"
#include "motors.hpp"


namespace Car
{
    // Custom deleter for m_gpio_map
    void delete_GPIO_map(volatile uint32_t* gpioMmap_ptr);

    class Car
    {
        private:
            std::shared_ptr<volatile uint32_t> m_gpioMmap;
            Sensors m_sensors;
            Motors  m_motors;
        public:
            Car();
            void initBeep();

    };

}


#endif