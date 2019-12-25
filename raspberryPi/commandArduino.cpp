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
    MotorController::serialPortFd = open(serialDevice.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (MotorController::serialPortFd == -1)
    {
        std::cerr << "ERROR: Could not open serial port \"" << serialDevice  << "\"\n"
                  << "Errno:" << std::strerror(errno) << "\n";
        std::exit(1);
    }
    if (fcntl(MotorController::serialPortFd, F_SETFL, FNDELAY) == -1)
    {
        std::cerr << "ERROR: fcntl() returned -1\n"
                  << "Errno:" << std::strerror(errno) << "\n";
        std::exit(1);
    }
    struct termios tty;
    std::memset(&tty, 0, sizeof(tty));
    if (tcgetattr(MotorController::serialPortFd, &tty) != 0)
    {
        std::cerr << "ERROR: tcgetattr() returned -1\n"
                  << "Errno:" << std::strerror(errno) << "\n";
        std::exit(1);
    }
    cfsetospeed(&tty, B9600);
    cfsetispeed(&tty, B9600);

    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;
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