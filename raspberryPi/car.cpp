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
    MotorController::init("/dev/ttyAMA0", baudRate);
    std::cout << "Motor controller initialized\n";
    this->setSpeed(100);
}

void Car::setSpeed(uint8_t speed)
{
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ <<" - About to set speed of:" << speed << "\n";
#endif
    MotorController::setSpeed(speed);
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ <<" - Post MotorController::setSpeed\n";
#endif
}

void Car::moveCar(CarMovement_t carDir)
{
    MotorDir_t leftSide, rightSide;

#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ <<" - carDir:" << carDir << "\n";
#endif

    switch(carDir)
    {
        case CarMovement_t::FORWARD:
            leftSide  = MotorDir_t::M_FORWARD;
            rightSide = MotorDir_t::M_FORWARD;
            break;
        case CarMovement_t::FORWARD_LEFT:
            leftSide  = MotorDir_t::M_RELEASE;
            rightSide = MotorDir_t::M_FORWARD;
            break;
        case CarMovement_t::FORWARD_RIGHT:
            leftSide  = MotorDir_t::M_FORWARD;
            rightSide = MotorDir_t::M_RELEASE;
            break;
        case CarMovement_t::REVERSE:
            leftSide  = MotorDir_t::M_REVERSE;
            rightSide = MotorDir_t::M_REVERSE;
            break;
        case CarMovement_t::REVERSE_LEFT:
            leftSide  = MotorDir_t::M_RELEASE;
            rightSide = MotorDir_t::M_REVERSE;
            break;
        case CarMovement_t::REVERSE_RIGHT:
            leftSide  = MotorDir_t::M_REVERSE;
            rightSide = MotorDir_t::M_RELEASE;
            break;
        case CarMovement_t::LEFT:
            leftSide  = MotorDir_t::M_REVERSE;
            rightSide = MotorDir_t::M_FORWARD;
            break;
        case CarMovement_t::RIGHT:
            leftSide  = MotorDir_t::M_FORWARD;
            rightSide = MotorDir_t::M_REVERSE;
            break;
        case CarMovement_t::STOP:
            leftSide  = MotorDir_t::M_RELEASE;
            rightSide = MotorDir_t::M_RELEASE;
            break;
        default:
            std::cerr << "moveCar() Invalid car direction:" 
                      << carDir << std::endl;
            return;
    }

#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ <<" - Pre MotorController::setDirection(" << leftSide << "," << rightSide << ")\n";
#endif
    MotorController::setDirection(leftSide, rightSide);
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ <<" - Post MotorController::setDirection()\n";
#endif
}

void Car::pan(uint8_t angle)
{
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ <<" - Pre MotorController::setPanServoAngle(" << angle << ")\n";
#endif
    MotorController::setPanServoAngle(angle);
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ <<" - Post MotorController::setPanServoAngle()\n";
#endif
}
void Car::tilt(uint8_t angle)
{
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ <<" - Pre MotorController::setTiltServoAngle(" << angle << ")\n";
#endif
    MotorController::setTiltServoAngle(angle);
#ifdef DEBUG
    std::cout << __PRETTY_FUNCTION__ <<" - Post MotorController::setTiltServoAngle()\n";
#endif
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
    std::exit(1);
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
    uint8_t outputBuf[messageSize];
    servoControl::serializePanServoAngle(angle, outputBuf);
    size_t nBytes = write(m_serialFd, outputBuf, messageSize); 
    if (nBytes != messageSize)
    {
        std::cerr << __PRETTY_FUNCTION__ << " write() returned "
                   << nBytes << "instead of " << messageSize << std::endl;
    }
}

void MotorController::setTiltServoAngle(uint8_t angle)
{
    if(!m_initialized)
        return;
    uint8_t outputBuf[messageSize];
    servoControl::serializeTiltServoAngle(angle, outputBuf);
    size_t nBytes = write(m_serialFd, outputBuf, messageSize); 
    if (nBytes != messageSize)
    {
        std::cerr << __PRETTY_FUNCTION__ << " write() returned "
                   << nBytes << "instead of " << messageSize << std::endl;
    }
}

void MotorController::setSpeed(uint8_t speed)
{
    if(!m_initialized)
        return;
    uint8_t outputBuf[messageSize];
    wheelControl::serializeWheelSpeed(speed, outputBuf);
    size_t nBytes = write(m_serialFd, outputBuf, messageSize); 
    if (nBytes != messageSize)
    {
        std::cerr << __PRETTY_FUNCTION__ << " write() returned "
                   << nBytes << "instead of " << messageSize << std::endl;
    }
}

void MotorController::setDirection(MotorDir_t leftSideDir, MotorDir_t rightSideDir)
{
    if(!m_initialized)
        return;

    uint8_t outputBuf[messageSize];
    wheelControl::serializeWheelDirs(leftSideDir, rightSideDir, outputBuf);
    size_t nBytes = write(m_serialFd, outputBuf, messageSize); 
    if (nBytes != messageSize)
    {
        std::cerr << __PRETTY_FUNCTION__ << " write() returned "
                   << nBytes << "instead of " << messageSize << std::endl;
    }
}

uint16_t MotorController::getUltrasonicDistance()
{
    if(!m_initialized)
        return 0;
    // Send any value to request input from the sensor
    uint8_t outputBuf[messageSize];
    ultrasonicInfo::serializeDistance(0, outputBuf);
    size_t nBytesWritten = write(m_serialFd, outputBuf, messageSize); 
    if (nBytesWritten != messageSize)
    {
        std::cerr << __PRETTY_FUNCTION__ << " write() returned "
                   << nBytesWritten << "instead of " << messageSize << std::endl;
        return 0;
    }

    while(serialDataAvail(m_serialFd) < static_cast<int>(messageSize))
    {
        // Wait until the Arduino has responded
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Data should be available
    uint8_t inputBuf[messageSize];
    size_t nBytesRead = read(m_serialFd, inputBuf, messageSize);
    if (nBytesRead == messageSize)
    {
        return ultrasonicInfo::deserializeDistance(inputBuf);
    }
    else
    {
        std::cerr << __PRETTY_FUNCTION__ << " read() returned "
                   << nBytesRead << "instead of " << messageSize << std::endl;
        return 0; // Default to 0
    }
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
