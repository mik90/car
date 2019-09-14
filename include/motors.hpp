#ifndef MOTORS_HPP_
#define MOTORS_HPP_

#include <memory>
#include <cerrno>
#include <cstdint>

#include "common_rpi.hpp"

// Note: Old code has DCMotorInit(), is this needed?;


namespace Car
{
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


    /** @brief All of the functions and data types relating to motor usage **/
    class Motors
    {
        private:
            std::shared_ptr<volatile uint32_t> m_gpioMmap;
            
            // Init motor and servo pins
            void initPanTilt();
            void initPwm();
            void initDcMotorController();

            // Control motors
            void writeToMotorRegister(uint8_t registerData);
            void turnDcMotor(bcm_pin_t motorPin, MotorDir_t motorDir);
        public:
            Motors();

            void setMemoryMap(const std::shared_ptr<volatile uint32_t>& gpioMmap_ptr);
            void moveCar(CarDirection_t carDir);
    };

}

#endif