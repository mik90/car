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

enum class CarDirection_t : std::uint8_t {FORWARD       = 0,
                                          FORWARD_RIGHT = 1,
                                          FORWARD_LEFT  = 2,
                                          REVERSE       = 3,
                                          REVERSE_LEFT  = 4,
                                          REVERSE_RIGHT = 5,
                                          LEFT          = 6,
                                          RIGHT         = 7};

// For printing out car directions
std::ostream& operator<<(std::ostream& out, CarDirection_t dir);

class Car
{
    private:
        // TODO Ensure order of construction
        RpiInterface m_rpi;
        Sensors      m_sensors; 
        Effectors    m_effectors;
    public:
        void moveCar(CarDirection_t carDir);
        void beepSeconds(std::chrono::seconds duration);
};

}


#endif