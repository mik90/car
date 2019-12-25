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
int MotorController::serialPortFd = -1;

using namespace motorControllerApi;

static void handleMotorControllerError(const std::string& funcName,
                                       const std::string& errorFunc,
                                       ssize_t nBytes)
{
    std::cerr << funcName << " " << errorFunc << " returned "
                << nBytes << " instead of " << messageSize << std::endl
                << "Errno:" << std::strerror(errno) << std::endl;
    std::exit(1);
}


void MotorController::init(const std::string& serialDevice, int baudRate)
{
    MotorController::serialPortFd = open(serialDevice.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (MotorController::serialPortFd == -1)
    {
        std::cerr << "ERROR: Could not open serial port \"" << serialDevice  << "\"\n"
                  << "Errno:" << std::strerror(errno) << "\n";
        std::exit(1);
    }
    if (fcntl(MotorController::serialPortFd, F_SETFL, 0) == -1)
    {
        std::cerr << "ERROR: fcntl() returned -1\n"
                  << "Errno:" << std::strerror(errno) << "\n";
        std::exit(1);
    }

    // Adruino resets when the serial port is connected, so we have to
    // give it some time before sending commmands
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

void MotorController::cleanUp()
{
    close(MotorController::serialPortFd);
}

void MotorController::setPanServoAngle(uint8_t angle)
{
    uint8_t outputBuf[messageSize];
    servoControl::serializePanServoAngle(angle, outputBuf);
    ssize_t nBytes = write(MotorController::serialPortFd, outputBuf, messageSize); 
    if (nBytes != messageSize)
        handleMotorControllerError(__PRETTY_FUNCTION__, "write()", nBytes);
}

void MotorController::setTiltServoAngle(uint8_t angle)
{
    uint8_t outputBuf[messageSize];
    servoControl::serializeTiltServoAngle(angle, outputBuf);
    ssize_t nBytes = write(MotorController::serialPortFd, outputBuf, messageSize); 

    if (nBytes != messageSize)
        handleMotorControllerError(__PRETTY_FUNCTION__, "write()", nBytes);
}

void MotorController::setSpeed(uint8_t speed)
{
    uint8_t outputBuf[messageSize];
    wheelControl::serializeWheelSpeed(speed, outputBuf);
    ssize_t nBytes = write(MotorController::serialPortFd, outputBuf, messageSize); 
    
    if (nBytes != messageSize)
        handleMotorControllerError(__PRETTY_FUNCTION__, "write()", nBytes);
}

void MotorController::setDirection(MotorDir_t leftSideDir, MotorDir_t rightSideDir)
{
    uint8_t outputBuf[messageSize];
    wheelControl::serializeWheelDirs(leftSideDir, rightSideDir, outputBuf);
    ssize_t nBytes = write(MotorController::serialPortFd, outputBuf, messageSize); 
    
    if (nBytes != messageSize)
        handleMotorControllerError(__PRETTY_FUNCTION__, "write()", nBytes);
}

uint16_t MotorController::getUltrasonicDistance()
{
    // Send any value to request input from the sensor
    uint8_t outputBuf[messageSize];
    ultrasonicInfo::serializeDistance(0, outputBuf);
    ssize_t nBytesWritten = write(MotorController::serialPortFd, outputBuf, messageSize); 
    
    if (nBytesWritten != messageSize)
        handleMotorControllerError(__PRETTY_FUNCTION__, "write()", nBytesWritten);

    int nBytesAvailable = 0;
    do
    {
        if (ioctl(MotorController::serialPortFd, FIONREAD, &nBytesAvailable) < 0)
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
    ssize_t nBytesRead = read(MotorController::serialPortFd, inputBuf, messageSize);
    if (nBytesRead == messageSize)
        return ultrasonicInfo::deserializeDistance(inputBuf);
    else
        handleMotorControllerError(__PRETTY_FUNCTION__, "read()", nBytesRead);

    // Should not reach this
    return 0;
}

}