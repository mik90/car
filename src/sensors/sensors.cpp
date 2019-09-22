#include "sensors.hpp"
#include "wiringPi.h"

namespace Car
{
    float Sensors::getDistanceUltrasonic()
    {
        return m_ultasonicDist_cm;
    }


    float Sensors::calcDistanceUltrasonic()
    {

        // Pull ultrasonic sensor high for at least 10 us
        digitalWrite(BCM::Trig, HIGH);
        delayMicroseconds(10);
        //  Pull ultrasonic trigger low again
        digitalWrite(BCM::Trig, LOW);
        
        // Keep reading the echo pin until it is high
        while (digitalRead(BCM::Echo) == 0);

        // Get wall clock time in microseconds
        unsigned long start_us = micros();

        // Keep reading the echo pin until it is low
        while (digitalRead(BCM::Echo) == 1);
        
        unsigned long pulseWidth_us = micros() - start_us;

        // Calculate distance in centimeters
        // Assumes speed of sound in air at sea level (340 m/s)
        float dist_cm = pulseWidth_us / 58.0;

        return dist_cm;
    }
}