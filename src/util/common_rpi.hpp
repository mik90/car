#ifndef COMMON_RPI_HPP_
#define COMMON_RPI_HPP_

#include <cstdint>

namespace Car
{
    using bcm_pin_t = unsigned int;

    // Conventional size of memory block
    const std::size_t BLOCK_SIZE = 4096;

    namespace GPIO
    {
        const unsigned long PI_ZERO_W_BCM2708_PERI_BASE = 0x20000000;
        const unsigned long PI_3_B_BCM2708_PERI_BASE    = 0x3f000000;
        const unsigned long PI_4_B_BCM2708_PERI_BASE    = 0xfe000000;
        const unsigned long GPIO_BASE = PI_3_B_BCM2708_PERI_BASE + 0x200000;
        
    }
    
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
        
        const bcm_pin_t MOTOR_LATCH = 29;
        const bcm_pin_t MOTOR_CLK   = 28;
        const bcm_pin_t MOTOR_DATA  = 27;
    }

    // Attempt to refactor INP_GPIO, OUT_GPIO and other weird macros that seem to be
    // all over the pi/arduino world
    inline void
    setPinInput(const bcm_pin_t pin,
                const std::shared_ptr<volatile uint32_t>& gpio_mmap_ptr)
    {
        // TODO - Explain this bit shifting
        auto adjusted_ptr = (gpio_mmap_ptr + ((pin) / 10));
        auto adjusted_pin = 7 << (((pin) % 10 ) * 3);
        *adjusted_ptr &= ~(adjusted_pin);
    }
    
    inline void
    setPinOutput(const bcm_pin_t pin,
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
    getGpioPullPtr(volatile uint32_t* gpio_mmap_ptr)
    {
        return gpio_mmap_ptr + 37;
    }
    
    /** @brief Gets offset for the pull up/down clock in memmory map
     *  @param[in] gpio_mmap - Points to base of memory mapped GPIO
     *  @return pointer to pull up/down clock. Used in order to modify
     *  the underlying memory content.**/
    inline volatile uint32_t*
    getGpioPullClk0Ptr(volatile uint32_t* gpio_mmap_ptr)
    {
        return gpio_mmap_ptr + 38;
    }


}
#endif