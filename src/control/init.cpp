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

    /*
    Only use the the DC motors for nwo
    this->initPanTilt();
    std::cout << "Pan/tilt servos initialized\n";
    */

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
    setPinInput(wPiPins::Servo_1, m_gpioMmap);
    setPinOutput(wPiPins::Servo_1, m_gpioMmap);

    setPinInput(wPiPins::Servo_2, m_gpioMmap);
    setPinOutput(wPiPins::Servo_2, m_gpioMmap);
}

/** @brief Sets up motor controller **/
void Motors::initDcMotorController()
{
    pinMode(wPiPins::MotorLatch, OUTPUT);
    pinMode(wPiPins::MotorData,  OUTPUT);
    pinMode(wPiPins::MotorClock,   OUTPUT);
    
    setPinInput (wPiPins::MotorPwmRR, m_gpioMmap);
    setPinOutput(wPiPins::MotorPwmRR, m_gpioMmap);
    
    setPinInput (wPiPins::MotorPwmRL, m_gpioMmap);
    setPinOutput(wPiPins::MotorPwmRL, m_gpioMmap);
    
    setPinInput (wPiPins::MotorPwmFR, m_gpioMmap);
    setPinOutput(wPiPins::MotorPwmFR, m_gpioMmap);
    
    setPinInput (wPiPins::MotorPwmFL, m_gpioMmap);
    setPinOutput(wPiPins::MotorPwmFL, m_gpioMmap);


    // Clear out the register
    writeToMotorRegister(0);
}


}

