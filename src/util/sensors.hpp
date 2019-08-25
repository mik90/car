#ifndef SENSORS_HPP_
#define SENSORS_HPP_

#include <iostream>
#include <cstdlib>
#include <memory>

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

#include <wiringPi.h>
#include <softPwm.h>

#include "constants.hpp"
#include "ws2811.h"

namespace Car
{
    using bcm_pin_t = unsigned int;

    // Custom deleter for m_gpio_map
    void delete_GPIO_map(volatile uint32_t* gpio_mmap_ptr);

    /** @brief All of the functions and data types relating to sensor usage **/
    class Sensors
    {
        private:
            static const unsigned char InfraredBufferLimit = 64;
            std::shared_ptr<volatile uint32_t> m_gpio_mmap;
        public:
            // shared_ptr is assigned a customer deleter here where the
            // mapped memory is unmapped
            Sensors();

            void init_pan_tilt();
            void init_ultrasonic();
            void init_line_reader();
            void init_beep();
            void init_infrared();
            void init_pwm();
    };

    /** @brief Controls the LED lights **/
    class RGB
    {
        private:
            // 4 LEDs wide
            const unsigned int LedRows = 4;
            // 4 LEDs tall 
            const unsigned int LedColumns = 4;
            const unsigned int LedCount = LedRows * LedColumns;

            std::unique_ptr<ws2811_led_t> m_led;
            ws2811_t m_led_data;
        public:
            // Allocates memory for LED matrix
            RGB();
    };

    /** @brief Commons place for all of the Pin identities **/
    namespace BCM
    {
        // BCM Pins? Some of these overlap
        /** @brief Beeper **/
        const bcm_pin_t Beep = 25;

        /** @brief Ultrasonic sensor output **/
        const bcm_pin_t Trig = 25;
        /** @brief Ultrasonic sensor input **/
        const bcm_pin_t Echo = 4;

        /** @brief Infrared sensor input **/
        const bcm_pin_t InfraredIn = 5;

        /** @brief Pan or tilt servo **/
        const bcm_pin_t Servo_1 = 4;
        /** @brief Pan or tilt servo **/
        const bcm_pin_t Servo_2 = 25;

        /** @brief Leftmost line tracker sensor **/
        const bcm_pin_t LineTrackLeft = 17;
        /** @brief Middle line tracker sensor **/
        const bcm_pin_t LineTrackMiddle = 27;
        /** @brief Rightmost line tracker sensor **/
        const bcm_pin_t LineTrackRight = 22;

        // PWM Motor controllers
        // These needs better labels wow. Im guessing FR FL RR RL?
        /** @brief Drive motor controller **/
        const bcm_pin_t MotorPWM_1 = 5;
        /** @brief Drive motor controller **/
        const bcm_pin_t MotorPWM_2 = 6;
        /** @brief Drive motor controller **/
        const bcm_pin_t MotorPWM_3 = 13;
        /** @brief Drive motor controller **/
        const bcm_pin_t MotorPWM_4 = 19;
    }



    // Attempt to refactor INP_GPIO, OUT_GPIO and other weird macros that seem to be
    // all over the pi/arduino world
    inline void
    set_pin_input(const bcm_pin_t pin,
                  const std::shared_ptr<volatile uint32_t>& gpio_mmap_ptr)
    {
        // TODO - Explain this bit shifting
        auto adjusted_ptr = (gpio_mmap_ptr.get() + ((pin) / 10));
        auto adjusted_pin = 7 << (((pin) % 10 ) * 3);
        *adjusted_ptr &= ~(adjusted_pin);
    }
    
    inline void
    set_pin_output(const bcm_pin_t pin,
                   const std::shared_ptr<volatile uint32_t>& gpio_mmap_ptr)
    {
        auto adjusted_ptr = (gpio_mmap_ptr.get() + ((pin) / 10));
        auto adjusted_pin = 1 << (((pin) % 10 ) * 3);
        *adjusted_ptr |= adjusted_pin;
    }

    /** @brief Gets offset for the pull up/down in memmory map
     *  @param[in] gpio_mmap - Points to base of memory mapped GPIO
     *  @return pointer to pull up/down. Used in order to modify
     *  the underlying memory content.**/
    inline volatile uint32_t*
    get_gpio_pull_ptr(const std::shared_ptr<volatile uint32_t>& gpio_mmap_ptr)
    {
        return gpio_mmap_ptr.get() + 37;
    }
    
    /** @brief Gets offset for the pull up/down clock in memmory map
     *  @param[in] gpio_mmap - Points to base of memory mapped GPIO
     *  @return pointer to pull up/down clock. Used in order to modify
     *  the underlying memory content.**/
    inline volatile uint32_t*
    get_gpio_pull_clk0_ptr(const std::shared_ptr<volatile uint32_t>& gpio_mmap_ptr)
    {
        return gpio_mmap_ptr.get() + 38;
    }
}

#endif