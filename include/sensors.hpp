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
            std::shared_ptr<volatile uint32_t> m_gpioMmap;
            float m_ultasonicDist_cm = 0.0;
            
            float getDistanceUltrasonic();

            // TODO Ensure that measurements are at least 60ms between invocations
            void readSensors();
            float calcDistanceUltrasonic();

            void initUltrasonic();
            void initLineReader();
            void initInfrared();
        public:
            Sensors();
            void setMemoryMap(const std::shared_ptr<volatile uint32_t>& m_gpioMmap);
            
    };




}

#endif