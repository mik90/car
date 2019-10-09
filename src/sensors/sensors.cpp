#include <cmath>
#include <thread>
#include <chrono>
#include "wiringPi.h"
#include "sensors.hpp"

namespace Car
{

cm Sensors::calcDistanceUltrasonic()
{
    // Pull ultrasonic sensor high for at least 10 us
    digitalWrite(wPiPins::Trig, HIGH);
    delayMicroseconds(10);
    //  Pull ultrasonic trigger low again
    digitalWrite(wPiPins::Trig, LOW);
    
    // Keep reading the echo pin until it is high
    while (digitalRead(wPiPins::Echo) == 0);

    // Get wall clock time in microseconds
    unsigned long start_us = micros();

    // Keep reading the echo pin until it is low
    while (digitalRead(wPiPins::Echo) == 1);
    
    unsigned long pulseWidth_us = micros() - start_us;

    // Calculate distance in centimeters
    // Assumes speed of sound in air at sea level (340 m/s)
    cm dist = std::round(pulseWidth_us / 58.0);

    return dist;
}

[[noreturn]] void Sensors::updateLoopUltrasonic(std::chrono::milliseconds loopInterval)
{
    using namespace std::chrono_literals;

    // The loop interval should be quicker than 60ms
    loopInterval = std::max(loopInterval, 60ms);

    std::cout << "Updating ultrasonic sensors every " << loopInterval.count() << "ms" << "\n";

    while(true)
    {
        std::this_thread::sleep_for(loopInterval);
        m_ultrasonicDist = calcDistanceUltrasonic();
    }

}


}