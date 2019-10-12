#ifndef COMMON_RPI_HPP_
#define COMMON_RPI_HPP_
#include <cstring>
#include <iostream>
#include <utility>

#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include <memory>
#include <bitset>
#include <iostream>
#include <cstdint>

#include "wiringPi.h"

namespace Car
{

using pin_t = unsigned int;

// Conventional size of memory block
constexpr std::size_t BLOCK_SIZE = 4096;

struct memMapDeleter
{
    void operator() (volatile uint32_t* gpioMmapPtr)
    {
        if (munmap((void*) gpioMmapPtr, BLOCK_SIZE) != 0)
            std::cerr << "Cannot unmap GPIO map. errno:" << std::strerror(errno) << '\n';
    }
};

class RpiInterface
{
    private:
        inline static std::unique_ptr<volatile uint32_t, memMapDeleter> m_gpioMmapPtr;
    public:
        RpiInterface();
        static void setPinInput(pin_t pin);
        static void setPinOutput(pin_t pin);
        static void writeToPullUpDown(uint32_t data);
        static void writeToPullUpDownClock(uint32_t data);
        static void writeGpioBits(std::bitset<32> bits);
        static void clearGpioBits(std::bitset<32> bits);
};

namespace GPIO
{
    constexpr unsigned int PI_ZERO_W_BCM2708_PERI_BASE = 0x20000000;
    constexpr unsigned int PI_3_B_BCM2708_PERI_BASE    = 0x3f000000;
    constexpr unsigned int PI_4_B_BCM2708_PERI_BASE    = 0xfe000000;
    constexpr unsigned int GPIO_BASE = PI_3_B_BCM2708_PERI_BASE + 0x200000;
    
}

/** @brief Common place for all of the WiringPi Pin identities **/
namespace wPiPins 
{
    // CONFLICT
    /** @brief Ultrasonic sensor input **/
    constexpr pin_t Echo = 4;
    /** @brief Pan or tilt servo **/
    constexpr pin_t Servo_1 = 4;

    // CONFLICT
    /** @brief Infrared sensor input **/
    constexpr pin_t InfraredIn = 5;

    /** @brief Leftmost line tracker sensor **/
    constexpr pin_t LineTrackLeft = 17;
    /** @brief Rightmost line tracker sensor **/
    constexpr pin_t LineTrackRight = 22;
    
    // CONFLICT
    /** @brief Ultrasonic sensor output **/
    constexpr pin_t Trig = 25;
    /** @brief Pan or tilt servo **/
    constexpr pin_t Servo_2 = 25;
    
    /** @brief Beeper **/
    constexpr pin_t Beep = 26;
    /** @brief Middle line tracker sensor **/
    constexpr pin_t LineTrackMiddle = 27;


    constexpr pin_t MotorData  = 27;
    constexpr pin_t MotorClock = 28;
    constexpr pin_t MotorLatch = 29;

    constexpr pin_t MotorPwmRR = 5;
    constexpr pin_t MotorPwmRL = 6;
    constexpr pin_t MotorPwmFR = 13;
    constexpr pin_t MotorPwmFL = 19;
}


}
#endif