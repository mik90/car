#ifndef SENSORS_HPP_
#define SENSORS_HPP_

#include <cstdlib>
#include <memory>


#include "common_rpi.hpp"

namespace Car
{

    /** @brief All of the functions and data types relating to sensor usage **/
    class Sensors
    {
        private:
            static const unsigned char InfraredBufferLimit = 64;
            float m_ultasonicDist_cm = 0.0;
            
            float getDistanceUltrasonic();

            // TODO Ensure that measurements are at least 60ms between invocations
            // TODO Use a centimeter type
            float calcDistanceUltrasonic();
        public:
            Sensors();
            
    };




}

#endif