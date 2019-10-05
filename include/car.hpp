#ifndef CAR_HPP_
#define CAR_HPP_

#include <cstdint>
#include <memory>

#include "sensors.hpp"
#include "motors.hpp"
#include "common_rpi.hpp"


namespace Car
{

class Car
{
    private:
        Sensors m_sensors;
        Motors  m_motors;
        RpiInterface m_rpi;
    public:
        Car();
        void moveCar(CarDirection_t carDir);
        void initBeep();
        void beepSeconds(unsigned int sec);

};

}


#endif