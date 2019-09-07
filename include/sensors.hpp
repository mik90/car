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
        public:
            // shared_ptr is assigned a customer deleter here where the
            // mapped memory is unmapped
            Sensors();

            void setMemoryMap(const std::shared_ptr<volatile uint32_t>& m_gpioMmap);
            void initUltrasonic();
            void initLineReader();
            void initInfrared();
    };




}

#endif