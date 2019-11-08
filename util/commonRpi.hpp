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

extern std::atomic<bool> wiringPiInitialized;

/** @brief Common place for all of the WiringPi Pin identities **/
namespace wPiPins 
{
    using pin_t = unsigned int;

    // Translated from BCM to WiringPi
    /** @brief Middle line tracker sensor **/
    constexpr pin_t LineTrackMiddle = 2;
    /** @brief Leftmost line tracker sensor **/
    constexpr pin_t LineTrackLeft = 0;
    /** @brief Rightmost line tracker sensor **/
    constexpr pin_t LineTrackRight = 3;

    constexpr pin_t PwmWheelRR = 21;
    constexpr pin_t PwmWheelRl = 22;
    constexpr pin_t PwmWheelFR = 23;
    constexpr pin_t PwmWheelFL = 24;
    
    /** @brief Camera pan servo **/
    constexpr pin_t PanServo = 7;

    /** @brief Camera tilt servo **/
    constexpr pin_t TiltServo = 6;
    
    /** @brief Listen for ultrasonic sensor input **/
    constexpr pin_t Echo = 4;
    /** @brief Trigger ultrasonic sensor output **/
    constexpr pin_t Trig = 25;
    
    /** @brief Infrared sensor input **/
    constexpr pin_t InfraredIn = 5;

    /** @brief Beeper/Buzzer **/
    constexpr pin_t Beep = 26;

    constexpr pin_t MotorData  = 27;
    constexpr pin_t MotorClock = 28;
    constexpr pin_t MotorLatch = 29;
}

}
#endif