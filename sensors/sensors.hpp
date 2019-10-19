#ifndef SENSORS_HPP_
#define SENSORS_HPP_

#include <cstdlib>
#include <memory>
#include <chrono>

#include "common_rpi.hpp"

namespace Car
{
    using centimeters = int; 
    using cm = centimeters;

    constexpr std::chrono::milliseconds ultrasonicInterval{300};

    /** @brief All of the functions and data types relating to sensor usage **/
    class Sensors
    {
        private:
            cm m_ultrasonicDist{0};
            cm calcDistanceUltrasonic();
            void updateLoopUltrasonic(std::chrono::milliseconds loopInterval);

        public:
            cm getDistanceUltrasonic();
            Sensors();
            
    };




}

#endif