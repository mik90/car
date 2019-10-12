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

enum class CarMovement_t {FORWARD_LEFT, FORWARD, FORWARD_RIGHT,
                          LEFT,         STOP,    RIGHT,
                          REVERSE_LEFT, REVERSE, REVERSE_RIGHT};

// Print out car directions
std::ostream& operator<<(std::ostream& out, CarMovement_t dir);

class Car
{
    private:
        RpiInterface m_rpi; 
        Sensors      m_sensors; 
        Effectors    m_effectors;
    public:
        void moveCar(CarMovement_t carDir);
        void beep(std::chrono::seconds duration);
};

}


#endif