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
#include "../ws2811/ws2811.h"

namespace Car
{
    using bcm_pin_t = unsigned int;

    // Attempt to refactor INP_GPIO, OUT_GPIO and other weird macros that seem to be
    // all over the pi/arduino world
    constexpr void set_pin_input(const bcm_pin_t pin, volatile unsigned int* gpio_map)
    {
        // TODO - Explain this bit shifting
        auto adjusted_ptr = (gpio_map + ((pin) / 10));
        auto adjusted_pin = 7 << (((pin) % 10 ) * 3);
        *adjusted_ptr &= ~(adjusted_pin);
    }
    
    constexpr void set_pin_output(const bcm_pin_t pin, volatile unsigned int* gpio_map)
    {
        auto adjusted_ptr = (gpio_map + ((pin) / 10));
        auto adjusted_pin = 1 << (((pin) % 10 ) * 3);
        *adjusted_ptr |= adjusted_pin;
    }

    /** @brief Gets offset for the pull up/down in memmory map
     *  @param[in] gpio_mmap - Points to base of memory mapped GPIO
     *  @return pointer to pull up/down  **/
    constexpr volatile unsigned int* get_gpio_pull_ptr(volatile unsigned int* gpio_mmap)
    {
        return gpio_mmap + 37;
    }
    
    /** @brief Gets offset for the pull up/down clock in memmory map
     *  @param[in] gpio_mmap - Points to base of memory mapped GPIO
     *  @return pointer to pull up/down clock **/
    constexpr volatile unsigned int* get_gpio_pull_clk0_ptr(volatile unsigned int* gpio_mmap)
    {
        return gpio_mmap + 38;
    }


    /** @brief All of the functions and data types relating to sensor usage **/
    class Sensors
    {
        public:
            static const unsigned char InfraredBufferLimit = 64;


            volatile unsigned int* init_GPIO(void);
            void cleanup_GPIO(volatile unsigned int* gpio_mmap);

            void init_pan_tilt(volatile unsigned int* gpio_mmap);

            void init_ultrasonic(void);

            void init_line_reader(volatile unsigned int* gpio_mmap);

            void init_beep();
            void init_infrared();

            void init_pwm(volatile unsigned int* gpio_mmap);
    };

    class RGB
    {
        // Amount of leds per row
        const unsigned int LedRows = 4;
        // Amount of leds per column
        const unsigned int LedColumns = 4;
        const unsigned int LedCount = LedRows * LedColumns;

        void init_rgb(ws2811_led_t* led_ptr);
        /*
        ws2811_t led_data = {.freq = WS2811_TARGET_FREQ, .dmanum = 10,
         .channel = { [0] = { .gpionum = 18,
                              .count = LedCount,
                              .invert = 0,
                              .brightness = 255,
                              .strip_type = WS2811_STRIP_RGB },
                      [1] = { .gpionum = 0,
                              .count = 0,
                              .invert = 0,
                              .brightness = 0, },
                              }, };
            */
    };


    namespace BCM
    {

        // BCM Pins? Some of these overlap
        const bcm_pin_t Beep = 25;

        // Ultrasonic sensor
        const bcm_pin_t Trig = 25;
        const bcm_pin_t Echo = 4;

        // Infrared sensor
        const bcm_pin_t InfraredIn = 5;

        // Pan/tilt
        const bcm_pin_t Servo_1 = 4;
        const bcm_pin_t Servo_2 = 25;

        // Line tracker
        const bcm_pin_t LineTrackLeft = 17;
        const bcm_pin_t LineTrackMiddle = 27;
        const bcm_pin_t LineTrackRight = 22;

        // PWM Motor controllers
        // These needs better labels wow. Im guessing FR FL RR RL?
        const bcm_pin_t MotorPWM_1 = 5;
        const bcm_pin_t MotorPWM_2 = 6;
        const bcm_pin_t MotorPWM_3 = 13;
        const bcm_pin_t MotorPWM_4 = 19;
    };
};

#endif