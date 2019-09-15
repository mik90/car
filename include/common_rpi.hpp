#ifndef COMMON_RPI_HPP_
#define COMMON_RPI_HPP_

#include <memory>
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

    enum class MotorDir_t {FORWARD, REVERSE, RELEASE};
    // These values correspond to bit numbers in the physical register
    enum class MotorBit_t : uint8_t {MOTOR_RR_A = 0b00001000,  // Bit 3
                                     MOTOR_RR_B = 0b00001000,  // Bit 2
                                     MOTOR_RL_A = 0b00010000,  // Bit 4
                                     MOTOR_RL_B = 0b00000010,  // Bit 1
                                     MOTOR_FR_A = 0b00000001,  // Bit 0
                                     MOTOR_FR_B = 0b01000000,  // Bit 6
                                     MOTOR_FL_A = 0b00100000,  // Bit 5
                                     MOTOR_FL_B = 0b10000000}; // Bit 7

    enum class CarDirection_t : std::uint8_t {FORWARD       = 0,
                                              FORWARD_RIGHT = 1,
                                              FORWARD_LEFT  = 2,
                                              REVERSE       = 3,
                                              REVERSE_LEFT  = 4,
                                              REVERSE_RIGHT = 5,
                                              LEFT          = 6,
                                              RIGHT         = 7};

    enum class Acceleration_t : std::uint8_t {ZERO = 0,
                                              HALF = 50,
                                              FULL = 100};



    
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
        // These needs better labels
        // It might be RR, RL, FR, FL
        /** @brief Drive motor controller **/
        const bcm_pin_t MotorPWM_RR = 5;
        /** @brief Drive motor controller **/
        const bcm_pin_t MotorPWM_RL = 6;
        /** @brief Drive motor controller **/
        const bcm_pin_t MotorPWM_FR = 13;
        /** @brief Drive motor controller **/
        const bcm_pin_t MotorPWM_FL = 19;
        
        const bcm_pin_t MOTOR_LATCH = 29;
        const bcm_pin_t MOTOR_CLK   = 28;
        const bcm_pin_t MOTOR_DATA  = 27;
    }

    // Attempt to refactor INP_GPIO, OUT_GPIO and other weird macros that seem to be
    // all over the pi/arduino world
    inline void
    setPinInput(const bcm_pin_t pin,
                const std::shared_ptr<volatile uint32_t>& gpioMmap_ptr)
    {
        // TODO - Explain this bit shifting
        auto adjusted_ptr = (gpioMmap_ptr.get() + ((pin) / 10));
        auto adjusted_pin = 7 << (((pin) % 10 ) * 3);
        *adjusted_ptr &= ~(adjusted_pin);
    }
    
    inline void
    setPinOutput(const bcm_pin_t pin,
                 const std::shared_ptr<volatile uint32_t>& gpioMmap_ptr)
    {
        auto adjusted_ptr = (gpioMmap_ptr.get() + ((pin) / 10));
        auto adjusted_pin = 1 << (((pin) % 10 ) * 3);
        *adjusted_ptr |= adjusted_pin;
    }

    /** @brief Gets offset for the pull up/down in memmory map
     *  @param[in] gpio_mmap - Points to base of memory mapped GPIO
     *  @return pointer to pull up/down. Used in order to modify
     *  the underlying memory content.**/
    inline volatile uint32_t*
    getGpioPullPtr(volatile uint32_t* gpioMmap_ptr)
    {
        return gpioMmap_ptr + 37;
    }
    
    /** @brief Gets offset for the pull up/down clock in memmory map
     *  @param[in] gpio_mmap - Points to base of memory mapped GPIO
     *  @return pointer to pull up/down clock. Used in order to modify
     *  the underlying memory content.**/
    inline volatile uint32_t*
    getGpioPullClk0Ptr(volatile uint32_t* gpioMmap_ptr)
    {
        return gpioMmap_ptr + 38;
    }


}
#endif