#include <sys/types.h>
#include <iostream>

#include "wiringPi.h"

#include "motors.hpp"
#include "common_rpi.hpp"

namespace Car
{


explicit void Motors::setMemoryMap(std::shared_ptr<volatile uint32_t> gpio_ptr) 
{
    m_gpio_mmap = gpio_ptr;

    // Once the memory is mapped, we can init all of our periphials
    this->initPanTilt();
    std::cout << "Pan/tilt servos initialized\n";
    this->initPwm();
    std::cout << "Pulse width modulation initialized\n";
    this->initMotorController();
    std::cout << "Motor controller initialized\n";
    
    std::cout << "Motor initializion successful." << std::endl;
}


Motors::Motors() : m_gpio_mmap(nullptr)
{
}

/** @brief Sets pins for pan/tilt servos **/
void Motors::initPanTilt()
{
    setPinInput(BCM::Servo_1, m_gpio_mmap);
    setPinOutput(BCM::Servo_1, m_gpio_mmap);

    setPinInput(BCM::Servo_2, m_gpio_mmap);
    setPinOutput(BCM::Servo_2, m_gpio_mmap);
}

/** @brief Sets pins for all the PWM motor controllers as outputs **/
void Motors::initPwm()
{
    setPinInput (BCM::MotorPWM_RR, m_gpio_mmap);
    setPinOutput(BCM::MotorPWM_RR, m_gpio_mmap);
    
    setPinInput (BCM::MotorPWM_RL, m_gpio_mmap);
    setPinOutput(BCM::MotorPWM_RL, m_gpio_mmap);
    
    setPinInput (BCM::MotorPWM_FR, m_gpio_mmap);
    setPinOutput(BCM::MotorPWM_FR, m_gpio_mmap);
    
    setPinInput (BCM::MotorPWM_FL, m_gpio_mmap);
    setPinOutput(BCM::MotorPWM_FL, m_gpio_mmap);
}

/** @brief Sets up motor controller **/
void Motors::initMotorController()
{
    wiringPiSetup();
    pinMode(BCM::MOTOR_LATCH, OUTPUT);
    pinMode(BCM::MOTOR_DATA,  OUTPUT);
    pinMode(BCM::MOTOR_CLK,   OUTPUT);
}


}

