#ifndef SENSORS_HPP_
#define SENSORS_HPP_

#include <iostream>
#include <cstdlib>

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

#include <wiringPi.h>
#include <softPwm.h>

#include "constants.hpp"

namespace Car
{
    /** @brief All of the functions and data types relating to sensor usage **/
    class Sensors
    {
        public:
            volatile unsigned int* init_GPIO();
            void cleanup_GPIO(void* gpio_map);

            void init_ultrasonic();
            void cleanup_ultrasonic();


    };
};

#endif