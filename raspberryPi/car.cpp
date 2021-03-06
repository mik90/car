#include <cerrno>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <thread>

#include "car.hpp"

namespace Car
{
using namespace msg;

Car::Car()
#ifdef USE_FACE_DETECTION
             : m_faceDetector(cv::String("lbpcascade_frontalface_improved.xml"))
#endif
{
    ArduinoInterface::init("/dev/ttyACM0", baudRate);
    std::cout << "Motor controller initialized\n";
    std::cout << "Ensure that user is part of the dialout group\n";
    this->setSpeed(0);
    this->moveCar(CarMovement_t::STOP);
}

#ifdef USE_FACE_DETETCTION
FaceDetector& Car::getFaceDetector()
{
    return m_faceDetector;
}
#endif

void Car::setSpeed(std::uint8_t speed)
{
    ArduinoInterface::setSpeed(speed);
}

void Car::moveCar(CarMovement_t carDir)
{
    using namespace msg;
    MotorDir_t leftSide  = MotorDir_t::M_INVALID_DIR;
    MotorDir_t rightSide = MotorDir_t::M_INVALID_DIR;

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
            leftSide  = MotorDir_t::M_RELEASE;
            rightSide = MotorDir_t::M_RELEASE;
            std::cerr << "moveCar() Invalid car direction:" << carDir << std::endl;
            return;
    }

    std::cout << "moveCar(" << carDir << ")\n";
    ArduinoInterface::setDirection(leftSide, rightSide);
}

void Car::pan(std::uint8_t angle)
{
    ArduinoInterface::setPanServoAngle(angle);
}
void Car::tilt(std::uint8_t angle)
{
    ArduinoInterface::setTiltServoAngle(angle);
}

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
        case CarMovement_t::STOP: out << "STOP"; break;
        default:
            out << "Invalid CarMovement_t as std::uint_8:" << static_cast<std::uint8_t>(dir) << std::endl;
            out.setstate(std::ios_base::failbit);
            break;
    }
    return out;
}


} // End of namespace Car
