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
    pinMode(wPiPins::Echo, INPUT);
    pinMode(wPiPins::Trig, OUTPUT);
}


/** @brief Sets pins for line reader sensors */
void Sensors::initLineReader()
{
    volatile uint32_t* gpio_pull_ptr = getGpioPullPtr(m_gpioMmap.get());

    setPinInput(wPiPins::LineTrackLeft, m_gpioMmap);
    *gpio_pull_ptr = 1 << wPiPins::LineTrackLeft;

    setPinInput(wPiPins::LineTrackMiddle, m_gpioMmap);
    *gpio_pull_ptr = 1 << wPiPins::LineTrackMiddle;

    setPinInput(wPiPins::LineTrackRight, m_gpioMmap);
    *gpio_pull_ptr = 1 << wPiPins::LineTrackRight;
}
/** @brief Sets pin for the infrared sensor as input. Sets the pull
 * up/down resistor as up, and sets ISR for falling edge. **/
void Sensors::initInfrared()
{
    pinMode(wPiPins::InfraredIn, INPUT);
    pullUpDnControl(wPiPins::InfraredIn, PUD_UP);

    // TODO This ISR can't even be doing anything, maybe we don't need it.
    //wiringPiISR(wPiPins::InfraredIn, INT_EDGE_FALLING, infrared_ISR);
}

}