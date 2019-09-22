#include <sys/types.h>
#include <iostream>

#include "wiringPi.h"

#include "motors.hpp"
#include "common_rpi.hpp"

namespace Car
{


void Motors::setMemoryMap(const std::shared_ptr<volatile uint32_t>& gpioMmap_ptr) 
{
    // Copy the shared_ptr
    m_gpioMmap = gpioMmap_ptr;

    // Once the memory is mapped, we can init all of our periphials
    this->initPanTilt();
    std::cout << "Pan/tilt servos initialized\n";
    this->initPwm();
    std::cout << "Pulse width modulation initialized\n";
    this->initDcMotorController();
    std::cout << "Motor controller initialized\n";
    
    std::cout << "Motor initializion successful." << std::endl;
}


Motors::Motors() : m_gpioMmap(nullptr)
{
}

Motors::~Motors() 
{
}

/** @brief Sets pins for pan/tilt servos **/
void Motors::initPanTilt()
{
    setPinInput(BCM::Servo_1, m_gpioMmap);
    setPinOutput(BCM::Servo_1, m_gpioMmap);

    setPinInput(BCM::Servo_2, m_gpioMmap);
    setPinOutput(BCM::Servo_2, m_gpioMmap);
}

/** @brief Sets pins for all the PWM motor controllers as outputs **/
void Motors::initPwm()
{
    setPinInput (BCM::MotorPWM_RR, m_gpioMmap);
    setPinOutput(BCM::MotorPWM_RR, m_gpioMmap);
    
    setPinInput (BCM::MotorPWM_RL, m_gpioMmap);
    setPinOutput(BCM::MotorPWM_RL, m_gpioMmap);
    
    setPinInput (BCM::MotorPWM_FR, m_gpioMmap);
    setPinOutput(BCM::MotorPWM_FR, m_gpioMmap);
    
    setPinInput (BCM::MotorPWM_FL, m_gpioMmap);
    setPinOutput(BCM::MotorPWM_FL, m_gpioMmap);
}

/** @brief Sets up motor controller **/
void Motors::initDcMotorController()
{
    pinMode(BCM::MOTOR_LATCH, OUTPUT);
    pinMode(BCM::MOTOR_DATA,  OUTPUT);
    pinMode(BCM::MOTOR_CLK,   OUTPUT);
}


}

