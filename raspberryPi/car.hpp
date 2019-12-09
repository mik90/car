#ifndef CAR_HPP_
#define CAR_HPP_

#include <cstdint>
#include <memory>
#include <chrono>


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
    public:
        void moveCar(CarMovement_t carDir);
        void pan(int angle);
        void tilt(int angle);
};


}


#endif