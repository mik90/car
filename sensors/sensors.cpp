#include <cmath>
#include <thread>
#include <chrono>
#include "wiringPi.h"
#include "sensors.hpp"

namespace Car
{


Sensors::Sensors()
{
     // Once the memroy is mapped, we can init all of our periphials
    std::cout << "Initializing sensors..." << std::endl;

    pinMode(wPiPins::Echo, INPUT);
    pinMode(wPiPins::Trig, OUTPUT);
    std::cout << "Ultrasonic sensor initialized\n";

    pinMode(wPiPins::LineTrackLeft, INPUT);
    RpiInterface::writeToPullUpDown(1 << wPiPins::LineTrackLeft);

    pinMode(wPiPins::LineTrackMiddle, INPUT);
    RpiInterface::writeToPullUpDown(1 << wPiPins::LineTrackMiddle);

    pinMode(wPiPins::LineTrackRight, INPUT);
    RpiInterface::writeToPullUpDown(1 << wPiPins::LineTrackRight);
    std::cout << "Line reader initialized\n";

    pinMode(wPiPins::InfraredIn, INPUT);
    pullUpDnControl(wPiPins::InfraredIn, PUD_UP);
    std::cout << "Infrared sensor initialized\n";
    
    std::cout << "Sensor initializion done.\n";

    std::cout << "Starting ultrasonic update loop..." << std::endl;

    // Update the ultrasonic sensor every 'ultraSonicInterval' milliseconds 
    std::thread loopThread{&Sensors::updateLoopUltrasonic, this, ultrasonicInterval};
    // Let this thread be independent
    loopThread.detach();
}



cm Sensors::calcDistanceUltrasonic()
{
    // Pull ultrasonic sensor high for at least 10 us
    digitalWrite(wPiPins::Trig, HIGH);
    delayMicroseconds(10);
    //  Pull ultrasonic trigger low again
    digitalWrite(wPiPins::Trig, LOW);
    
    while (digitalRead(wPiPins::Echo) == 0)
    {
        // Keep reading the echo pin until it is high
    }

    // Get wall clock time in microseconds
    unsigned long start_us = micros();

    while (digitalRead(wPiPins::Echo) == 1)
    {
        // Keep reading the echo pin until it is low
    }
    
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