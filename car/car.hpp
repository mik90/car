#ifndef CAR_HPP_
#define CAR_HPP_

#include <cstdint>
#include <memory>
#include <chrono>

#include "commonRpi.hpp"

#include "sensorController.hpp"
#include "motorController.hpp"



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
        MotorController m_motors;
        SensorController         m_sensors; 
        void parseIrCommand(uint16_t command);
    public:
        void moveCar(CarMovement_t carDir);
        void beep(std::chrono::seconds duration);
        void pan(dutyCycle angle);
        void tilt(dutyCycle angle);
};


}


#endif