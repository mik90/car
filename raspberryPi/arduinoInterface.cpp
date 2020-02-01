#include <iostream>
#include <thread>

#include <cerrno>
#include <cstring>

#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "../common/messages.hpp"
#include "car.hpp"

namespace Car
{
// Initialize the private static member variables
int ArduinoInterface::serialPortFd = -1;

using namespace msg;

std::ostream& operator<<(std::ostream& out, MotorDir_t dir);

static void handleMotorControllerError(const std::string& funcName,
                                       const std::string& errorFunc,
                                       ssize_t nBytes)
{
    std::cerr << funcName << " " << errorFunc << " returned "
                << nBytes << " instead of " << messageSize << std::endl
                << "Errno:" << std::strerror(errno) << std::endl;
    std::exit(1);
}


void ArduinoInterface::init(const std::string& serialDevice, int baudRate)
{
    ArduinoInterface::serialPortFd = open(serialDevice.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (ArduinoInterface::serialPortFd == -1)
    {
        std::cerr << "ERROR: Could not open serial port \"" << serialDevice  << "\"\n"
                  << "Errno:" << std::strerror(errno) << "\n";
        std::exit(1);
    }

    std::cout << "Waiting for Arduino to restart..." << std::endl;
    // Adruino resets when the serial port is connected, so we have to
    // give it some time before sending commmands
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Arduino should be ready" << std::endl;
}

void ArduinoInterface::cleanUp()
{
    close(ArduinoInterface::serialPortFd);
}

void ArduinoInterface::setPanServoAngle(uint8_t angle)
{
    uint8_t outputBuf[messageSize];
    serializePanServoAngle(angle, outputBuf);
    ssize_t nBytes = write(ArduinoInterface::serialPortFd, outputBuf, messageSize); 
    if (nBytes != messageSize)
        handleMotorControllerError(__PRETTY_FUNCTION__, "write()", nBytes);
}

void ArduinoInterface::setTiltServoAngle(uint8_t angle)
{
    uint8_t outputBuf[messageSize];
    serializeTiltServoAngle(angle, outputBuf);
    ssize_t nBytes = write(ArduinoInterface::serialPortFd, outputBuf, messageSize); 

    if (nBytes != messageSize)
        handleMotorControllerError(__PRETTY_FUNCTION__, "write()", nBytes);
}

void ArduinoInterface::setSpeed(uint8_t speed)
{
    uint8_t outputBuf[messageSize];
    serializeWheelSpeed(speed, outputBuf);
    ssize_t nBytes = write(ArduinoInterface::serialPortFd, outputBuf, messageSize); 
    
    if (nBytes != messageSize)
        handleMotorControllerError(__PRETTY_FUNCTION__, "write()", nBytes);
}

void ArduinoInterface::setDirection(msg::MotorDir_t leftSideDir, msg::MotorDir_t rightSideDir)
{
    uint8_t outputBuf[messageSize];
    serializeWheelDirs(leftSideDir, rightSideDir, outputBuf);
    ssize_t nBytes = write(ArduinoInterface::serialPortFd, outputBuf, messageSize); 
    
    if (nBytes != messageSize)
        handleMotorControllerError(__PRETTY_FUNCTION__, "write()", nBytes);
}

uint16_t ArduinoInterface::getUltrasonicDistance()
{
    // Send any value to request input from the sensor
    uint8_t outputBuf[messageSize];
    serializeDistance(0, outputBuf);
    ssize_t nBytesWritten = write(ArduinoInterface::serialPortFd, outputBuf, messageSize); 
    
    if (nBytesWritten != messageSize)
        handleMotorControllerError(__PRETTY_FUNCTION__, "write()", nBytesWritten);

    int nBytesAvailable = 0;
    do
    {
        if (ioctl(ArduinoInterface::serialPortFd, FIONREAD, &nBytesAvailable) < 0)
        {
            // Couldn't read serial port
            std::cerr << "ERROR: ioctl() on serial port failed." << std::endl
                      << "Errno:" << std::strerror(errno) << std::endl;
            std::exit(1);
        }
        // Give the arduino time to respond
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

      // Ensure that there's at least a message's worth of data available
    } while(nBytesAvailable < static_cast<int>(messageSize));

    // Data should be available
    uint8_t inputBuf[messageSize];
    ssize_t nBytesRead = read(ArduinoInterface::serialPortFd, inputBuf, messageSize);
    if (nBytesRead == messageSize)
        return deserializeDistance(inputBuf);
    else
        handleMotorControllerError(__PRETTY_FUNCTION__, "read()", nBytesRead);

    // Should not reach this
    return 0;
}

std::ostream& operator<<(std::ostream& out, MotorDir_t dir)
{
    switch(dir)
    {
        case MotorDir_t::M_INVALID_DIR: out << "M_INVALID_DIR"; break;
        case MotorDir_t::M_FORWARD: out << "M_FORWARD"; break;
        case MotorDir_t::M_BRAKE: out << "M_BRAKE"; break;
        case MotorDir_t::M_RELEASE: out << "M_RELEASE"; break;
        case MotorDir_t::M_REVERSE: out << "M_REVERSE"; break;
        default:
            out << "Invalid MotorDir_t as std::uint_8:" << static_cast<std::uint8_t>(dir) << std::endl;
            out.setstate(std::ios_base::failbit);
            break;
    }
    return out;
}


}