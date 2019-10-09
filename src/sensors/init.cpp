#include <iostream>
#include <thread>

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

    RpiInterface::setPinInput(wPiPins::LineTrackLeft);
    RpiInterface::writeToPullUpDown(1 << wPiPins::LineTrackLeft);

    RpiInterface::setPinInput(wPiPins::LineTrackMiddle);
    RpiInterface::writeToPullUpDown(1 << wPiPins::LineTrackMiddle);

    RpiInterface::setPinInput(wPiPins::LineTrackRight);
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

}