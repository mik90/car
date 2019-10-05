#ifndef CAR_HPP_
#define CAR_HPP_

#include <cstdint>
#include <memory>
#include <chrono>

#include "sensors.hpp"
#include "effectors.hpp"
#include "common_rpi.hpp"


namespace Car
{

class Car
{
    private:
        Sensors m_sensors;
        Effectors  m_effectors;
        RpiInterface m_rpi;
    public:
        void moveCar(CarDirection_t carDir);
        void beepSeconds(std::chrono::seconds duration);
};

}


#endif