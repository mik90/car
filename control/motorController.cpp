#include <iostream>
#include <bitset>
#include <thread>
#include <atomic>

#include "wiringPi.h"

#include "motorController.hpp"

namespace Car
{

MotorController::MotorController()
{
    // Set pins for the UCTronics motor register with wiringPi
    pinMode(wPiPins::MotorLatch, OUTPUT);
    pinMode(wPiPins::MotorData,  OUTPUT);
    pinMode(wPiPins::MotorClock, OUTPUT);

    // Set pins for the GPIO Pulse width modulation controls
    pinMode(wPiPins::PwmWheelRR, OUTPUT);
    pinMode(wPiPins::PwmWheelRl, OUTPUT);
    pinMode(wPiPins::PwmWheelFR, OUTPUT);
    pinMode(wPiPins::PwmWheelFL, OUTPUT);

    // Set pins for pan/tilt servos
    pinMode(wPiPins::PanServo, OUTPUT);
    pinMode(wPiPins::TiltServo, OUTPUT);

    // Set pins/write low for the beeper/buzzer with wiringPi
    pinMode(wPiPins::Beep, OUTPUT);

    digitalWrite(wPiPins::Beep, LOW);
}


void MotorController::setWheelSpeed(PWM::pulseLength pLength)
{
    m_RearLeft.setSpeed(pLength);
    m_FrontLeft.setSpeed(pLength);
    
    m_RearRight.setSpeed(pLength);
    m_FrontRight.setSpeed(pLength);
}

void MotorController::turnWheels(MotorDir_t leftSide, MotorDir_t rightSide)
{
    // Logical OR all of the commands together
    std::bitset<8> motorCommand{m_RearLeft.calcMotorDirCommand(leftSide)
                                | m_FrontLeft.calcMotorDirCommand(leftSide)
                                | m_RearRight.calcMotorDirCommand(rightSide)
                                | m_FrontRight.calcMotorDirCommand(rightSide)};

    m_RearLeft.outputPwmCommand();
    m_FrontLeft.outputPwmCommand();
    m_RearRight.outputPwmCommand();
    m_FrontRight.outputPwmCommand();
    outputAllWheelCommands(motorCommand);
}

void MotorController::outputAllWheelCommands(std::bitset<8> motorCommands)
{
    digitalWrite(wPiPins::MotorLatch, LOW);
    digitalWrite(wPiPins::MotorData, LOW);

    // 'bitmask' is a bit that shifts right every iteration.
    // starts at the leftmost (7th) bit and ends at the rightmost (0th) bit
    for (std::bitset<8> bitmask{0b10000000}; !bitmask.none(); bitmask >>= 1)
    {
        delayMicroseconds(1);

        digitalWrite(wPiPins::MotorClock, LOW);

        // We can only tell one motor to turn at a time via the
        // MotorData signal.

        // Write high if 1, write low if 0
        if ((motorCommands & bitmask).any())
            digitalWrite(wPiPins::MotorData, HIGH);
        else
            digitalWrite(wPiPins::MotorData, LOW);

        delayMicroseconds(1);
        digitalWrite(wPiPins::MotorClock, HIGH);
    }

    digitalWrite(wPiPins::MotorLatch, HIGH);
}

void MotorController::pan(dutyCycle angle)
{
    m_panServo.m_angle = angle;
    m_panServo.turn(angle);
}

void MotorController::tilt(dutyCycle angle)
{
    m_tiltServo.m_angle = angle;
    m_tiltServo.turn(angle);
}

#if 0 //old crap 
void MotorController::panLeft()
{
    // Increase angle by 50, cap at 2300
    if ((m_panServo.m_angle + 50) < panLeftLimit)
    {
        m_panServo.m_angle += 50;
        m_panServo.turn(m_panServo.m_angle);
    }
}
void MotorController::panRight()
{
    // Decrease angle by 50, limit is 300
    if ((m_panServo.m_angle - 50) > panRightLimit)
    {
        m_panServo.m_angle -= 50;
        m_panServo.turn(m_panServo.m_angle);
    }
}

// TODO This is the only one that works for some reason
void MotorController::tiltUp()
{
    // Increase angle by 50, cap at 1160
    if ((m_tiltServo.m_angle + 50) > tiltUpLimit)
    {
        m_tiltServo.m_angle += 50;
        m_tiltServo.turn(m_tiltServo.m_angle);
    }
}
void MotorController::tiltDown()
{
    // Decrease angle by 50, cap at 1160
    if ((m_tiltServo.m_angle - 50) > tiltDownLimit)
    {
        m_tiltServo.m_angle -= 50;
        m_tiltServo.turn(m_tiltServo.m_angle);
    }
}
#endif


} // End of namespace