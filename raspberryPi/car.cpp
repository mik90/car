#include <cerrno>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <thread>

#include <unistd.h>

#include "wiringPi.h"
#include "wiringSerial.h"

#include "../common/messages.hpp"
#include "car.hpp"

namespace Car
{
// Initialize the private static member variables
bool MotorController::m_initialized = false;
int MotorController::m_serialFd = -1;

using namespace motorControllerApi;
using std::uint8_t;
using std::uint32_t;

Car::Car()
{
    MotorController::init("/dev/ttyACM0", baudRate);
    std::cout << "Motor controller initialized\n";
    this->setSpeed(100);
}

void Car::setSpeed(uint8_t speed)
{
    MotorController::setSpeed(speed);
}

void Car::moveCar(CarMovement_t carDir)
{
    MotorDir_t leftSide, rightSide;

    switch(carDir)
    {
        case CarMovement_t::FORWARD:
            leftSide  = MotorDir_t::FORWARD;
            rightSide = MotorDir_t::FORWARD;
            break;
        case CarMovement_t::FORWARD_LEFT:
            leftSide  = MotorDir_t::RELEASE;
            rightSide = MotorDir_t::FORWARD;
            break;
        case CarMovement_t::FORWARD_RIGHT:
            leftSide  = MotorDir_t::FORWARD;
            rightSide = MotorDir_t::RELEASE;
            break;
        case CarMovement_t::REVERSE:
            leftSide  = MotorDir_t::REVERSE;
            rightSide = MotorDir_t::REVERSE;
            break;
        case CarMovement_t::REVERSE_LEFT:
            leftSide  = MotorDir_t::RELEASE;
            rightSide = MotorDir_t::REVERSE;
            break;
        case CarMovement_t::REVERSE_RIGHT:
            leftSide  = MotorDir_t::REVERSE;
            rightSide = MotorDir_t::RELEASE;
            break;
        case CarMovement_t::LEFT:
            leftSide  = MotorDir_t::REVERSE;
            rightSide = MotorDir_t::FORWARD;
            break;
        case CarMovement_t::RIGHT:
            leftSide  = MotorDir_t::FORWARD;
            rightSide = MotorDir_t::REVERSE;
            break;
        case CarMovement_t::STOP:
            leftSide  = MotorDir_t::RELEASE;
            rightSide = MotorDir_t::RELEASE;
            break;
        default:
            std::cerr << "moveCar() Invalid car direction:" 
                      << carDir << std::endl;
            return;
    }

    // TODO - Send instructions to Arduino
    MotorController::setDirection(leftSide, rightSide);
}

void Car::pan(uint8_t angle)
{
    MotorController::setPanServoAngle(angle);
}
void Car::tilt(uint8_t angle)
{
    MotorController::setTiltServoAngle(angle);
}

// MotorController implementation below

void MotorController::init(const std::string& serialDevice, int baudRate)
{
  m_initialized = false;
  int m_serialFd = serialOpen(serialDevice.c_str(), baudRate);

  if (m_serialFd == -1)
  {
    std::cerr << "Could not open serial port:" << serialDevice  << "\n"
              << "Errno:" << std::strerror(errno) << "\n";
    return;
  }
  else
  {
      m_initialized = true;
  }
}

void MotorController::cleanUp()
{
    if(!m_initialized)
        return;
  serialClose(m_serialFd);
}

void MotorController::setPanServoAngle(uint8_t angle)
{
    if(!m_initialized)
        return;
    auto outputAr = servoControl::serializePanServoAngle(angle);
    write(m_serialFd, outputAr.data(), messageSize); 
}

void MotorController::setTiltServoAngle(uint8_t angle)
{
    if(!m_initialized)
        return;
    auto outputAr = servoControl::serializeTiltServoAngle(angle);
    write(m_serialFd, outputAr.data(), messageSize); 
}

void MotorController::setSpeed(uint8_t speed)
{
    if(!m_initialized)
        return;
    auto outputAr = wheelControl::serializeWheelSpeed(speed);
    write(m_serialFd, outputAr.data(), messageSize); 
}

void MotorController::setDirection(MotorDir_t leftSideDir, MotorDir_t rightSideDir)
{
    if(!m_initialized)
        return;

    auto outputAr = wheelControl::serializeWheelDirs(leftSideDir, rightSideDir);
    write(m_serialFd, outputAr.data(), messageSize); 
}

uint16_t MotorController::getUltrasonicDistance()
{
    if(!m_initialized)
        return 0;
    // Send any value to request input from the sensor
    auto requestAr = ultrasonicInfo::serializeDistance(0);
    write(m_serialFd, requestAr.data(), messageSize); 

    while(serialDataAvail(m_serialFd) < messageSize)
    {
        // Wait until the Arduino has responded
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Data should be available
    std::array<uint8_t, messageSize> inputAr;
    if (read(m_serialFd, inputAr.data(), messageSize) == 0)
        return ultrasonicInfo::deserializeDistance(inputAr);
    else
        return 0; // Default to 0
}

// Misc below ---

std::ostream& operator<<(std::ostream& out, CarMovement_t dir)
{
    switch(dir)
    {
        case CarMovement_t::FORWARD: out << "FORWARD"; break;
        case CarMovement_t::FORWARD_LEFT: out << "FORWARD_LEFT"; break;
        case CarMovement_t::FORWARD_RIGHT: out << "FORWARD_RIGHT"; break;
        case CarMovement_t::REVERSE: out << "REVERSE"; break;
        case CarMovement_t::REVERSE_LEFT: out << "REVERSE_LEFT"; break;
        case CarMovement_t::REVERSE_RIGHT: out << "REVERSE_RIGHT"; break;
        case CarMovement_t::LEFT: out << "LEFT"; break;
        case CarMovement_t::RIGHT: out << "RIGHT"; break;
        default:
            out << "Invalid CarMovement_t as uint_8:" << static_cast<uint8_t>(dir);
            out.setstate(std::ios_base::failbit);
            break;
    }
    return out;
}


} // End of namespace Car
