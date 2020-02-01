#ifndef CAR_HPP_
#define CAR_HPP_

#include <cstdint>
#include <memory>
#include <chrono>

#ifdef USE_FACE_DETECTION
#include "faceDetector.hpp"
#endif
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
#ifdef USE_FACE_DETECTION
      FaceDetector m_faceDetector;
#endif
    public:
      Car();
      void setSpeed(uint8_t speed);
      void moveCar(CarMovement_t carDir);
      void pan(uint8_t angle);
      void tilt(uint8_t angle);
#ifdef USE_FACE_DETECTION
      FaceDetector& getFaceDetector();
#endif
};

class ArduinoInterface
{
  public:
    static void init(const std::string& serialDevice, int baudRate);
    static void cleanUp();
    static void setPanServoAngle(uint8_t angle);
    static void setTiltServoAngle(uint8_t angle);
    static void setSpeed(uint8_t speed);
    static void setDirection(msg::MotorDir_t leftSideDir,
                             msg::MotorDir_t rightSideDir);
    static uint16_t getUltrasonicDistance();
  private:
    static int serialPortFd;
};

}

#endif
