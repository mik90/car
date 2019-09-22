#include <iostream>
#include <cerrno>
#include <cstring>

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>

#include "wiringPi.h"
#include "softPwm.h"

#include "sensors.hpp"
#include "common_rpi.hpp"

namespace Car
{

Sensors::Sensors() : m_gpioMmap(nullptr)
{
}

void Sensors::setMemoryMap(const std::shared_ptr<volatile uint32_t>& gpioMmap_ptr)
{
    // Copy the shared_ptr
    m_gpioMmap = gpioMmap_ptr;

     // Once the memroy is mapped, we can init all of our periphials
    std::cout << "Initializing sensors..." << std::endl;

    this->initUltrasonic();
    std::cout << "Ultrasonic sensor initialized\n";
    this->initLineReader();
    std::cout << "Line reader initialized\n";
    this->initInfrared();
    std::cout << "Infrared sensor initialized\n";
    
    std::cout << "Sensor initializion successful." << std::endl;
}


/** @brief Sets ultrasonic sensor pins **/
void Sensors::initUltrasonic()
{
    // This needs to be more clear than "Echo" and "Trig"
    pinMode(BCM::Echo, INPUT);
    pinMode(BCM::Trig, OUTPUT);
}


/** @brief Sets pins for line reader sensors */
void Sensors::initLineReader()
{
    volatile uint32_t* gpio_pull_ptr = getGpioPullPtr(m_gpioMmap.get());

    setPinInput(BCM::LineTrackLeft, m_gpioMmap);
    *gpio_pull_ptr = 1 << BCM::LineTrackLeft;

    setPinInput(BCM::LineTrackMiddle, m_gpioMmap);
    *gpio_pull_ptr = 1 << BCM::LineTrackMiddle;

    setPinInput(BCM::LineTrackRight, m_gpioMmap);
    *gpio_pull_ptr = 1 << BCM::LineTrackRight;
}
/** @brief Sets pin for the infrared sensor as input. Sets the pull
 * up/down resistor as up, and sets ISR for falling edge. **/
void Sensors::initInfrared()
{
    pinMode(BCM::InfraredIn, INPUT);
    pullUpDnControl(BCM::InfraredIn, PUD_UP);

    // TODO This ISR can't even be doing anything, maybe we don't need it.
    //wiringPiISR(BCM::InfraredIn, INT_EDGE_FALLING, infrared_ISR);
}

}