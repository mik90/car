#include <sys/types.h>
#include <iostream>

#include "wiringPi.h"

#include "effectors.hpp"
#include "common_rpi.hpp"

namespace Car
{
PwmMotor::PwmMotor(UCTronicsPins::pin_t pin) :
                                       m_pwmPin{pin},
                                       m_time{std::chrono::microseconds{0},
                                       false}
{
    // The motor bit is the pin-th index in the bitset set to 1 while all
    // others are set to 0
    m_pwmIdentityBitset = 1 << pin;
    
    // Default speed is medium
    m_pulseLength = PWM::Speed::Medium;

    // These values are kind of arbitrary, they're pulled from
    // the UCTRONICS codebase and have no explanation
    switch(pin)
    {
        case UCTronicsPins::PwmMotorRR:
            m_motorForward = 0b00001000;  // Bit 3
            m_motorReverse = 0b00000100;  // Bit 2
            break;
        case UCTronicsPins::PwmMotorRl:
            m_motorForward = 0b00010000;  // Bit 4
            m_motorReverse = 0b00000010;  // Bit 1
            break;
        case UCTronicsPins::PwmMotorFR:
            m_motorForward = 0b00100000;  // Bit 5
            m_motorReverse = 0b10000000;  // Bit 7
            break;
        case UCTronicsPins::PwmMotorFL:
            m_motorForward = 0b00000001;  // Bit 0
            m_motorReverse = 0b01000000;  // Bit 6
            break;
        default:
            std::cerr << "PwmMotor() invalid pin\n";
            m_motorForward = 0b00000000;
            m_motorReverse = 0b00000000;
            break;
    }
}

// Effectors implementation

Effectors::Effectors()
{
    // Set pins for the UCTronics motor register with wiringPi
    pinMode(wPiPins::MotorLatch, OUTPUT);
    pinMode(wPiPins::MotorData,  OUTPUT);
    pinMode(wPiPins::MotorClock, OUTPUT);

    // Set pins for the GPIO Pulse width modulation controls
    RpiInterface::setPinInput (UCTronicsPins::PwmMotorRR);
    RpiInterface::setPinOutput(UCTronicsPins::PwmMotorRR);
    
    RpiInterface::setPinInput (UCTronicsPins::PwmMotorRl);
    RpiInterface::setPinOutput(UCTronicsPins::PwmMotorRl);
    
    RpiInterface::setPinInput (UCTronicsPins::PwmMotorFR);
    RpiInterface::setPinOutput(UCTronicsPins::PwmMotorFR);
    
    RpiInterface::setPinInput (UCTronicsPins::PwmMotorFL);
    RpiInterface::setPinOutput(UCTronicsPins::PwmMotorFL);

    // Set pins for pan/tilt servos
    //RpiInterface::setPinInput (UCTronicsPins::PanServo);
    RpiInterface::setPinInput (UCTronicsPins::PanServo);
    RpiInterface::setPinOutput(UCTronicsPins::PanServo);

    RpiInterface::setPinInput (UCTronicsPins::TiltServo);
    RpiInterface::setPinOutput(UCTronicsPins::TiltServo);

    // Set pins/write low for the beeper/buzzer with wiringPi
    pinMode(wPiPins::Beep, OUTPUT);
    digitalWrite(wPiPins::Beep, LOW);
}

}
