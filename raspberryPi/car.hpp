#ifndef CAR_HPP_
#define CAR_HPP_

#include <cstdint>
#include <memory>
#include <chrono>

#include "../common/messages.hpp"

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
        Car();
        void setSpeed(uint8_t speed);
        void moveCar(CarMovement_t carDir);
        void pan(uint8_t angle);
        void tilt(uint8_t angle);
};

class MotorController
{
  public:
    static void init(const std::string& serialDevice, int baudRate);
    static void cleanUp();
    static void setPanServoAngle(uint8_t angle);
    static void setTiltServoAngle(uint8_t angle);
    static void setSpeed(uint8_t speed);
    static void setDirection(motorControllerApi::MotorDir_t leftSideDir,
                             motorControllerApi::MotorDir_t rightSideDir);
    static uint16_t getUltrasonicDistance();
  private:
    static int  m_serialFd;
    static bool m_initialized;
};

}

#endif
