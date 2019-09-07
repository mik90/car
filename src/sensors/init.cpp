
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


#ifdef LEGACY
/** @brief This was originally countLow() in remoteserver.c but it doesn't
 *  seem to make much sense **/
unsigned char count_infrared(void)
{
    unsigned char count = 0;
    while (digitalRead(BCM::InfraredIn) == 0)
    {
        // Wait for it to go high? I thought this was reading low...
    }

    while (digitalRead(BCM::InfraredIn) == 1)
    {
        count++;
        // 26 seems arbitrary, why was this chosen?
        // I guess it's just incrementing the count every 26 microseconds
        // while the signal is high
        delayMicroseconds(26);
    }

    return count;
}

// TODO These need to be cleaned up. I can't even find where they're
// used in the old remoteserver.c file
// Bits is a global variable in the original code
unsigned char bits = 0;
// Buffer is a global variable in the original code
unsigned char buffer[Sensors::InfraredBufferLimit];

bool infrared_ISR_done = false;
/** @brief Interrupt service routine for the infrared sensor.
 *  Triggered on the falling edge.
 *  @note I really doubt this code actually does anything in the
 *  old repo **/
void infrared_ISR(void)
{
    const unsigned char BitsPerByte = 8;
    const unsigned char initialCount = count_infrared();
    unsigned char count = 0;
    bits = 0;

    for (unsigned char i = 0; i < Sensors::InfraredBufferLimit; i++)
    {
        for (unsigned char j = 0; j < BitsPerByte; j++)
        {
            count = count_infrared();
            if (count == 0)
            {
                buffer[i] >>= (8 - j);
                infrared_ISR_done = true;
                return;
            }

            buffer[i] >>= 1;

            // Once again, arbitrary number
            // Look at the original code for this though, it's hilarious
            if (count > 30)
                buffer[i] += 0x80;

            bits++;
        }
    }
    infrared_ISR_done = true;
}
#endif