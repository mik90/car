#include <sys/types.h>

#include "motors.hpp"
#include "constants.hpp"

namespace Car
{


explicit Motors::Motors(std::shared_ptr<volatile uint32_t> gpio_ptr) 
                           : m_gpio_mmap(std::move(gpio_ptr)))
{
    // Once the memroy is mapped, we can init all of our periphials
    this->init_pan_tilt();
    std::cout << "Pan/tilt servos initialized\n";
    this->init_pwm();
    std::cout << "Pulse width modulation initialized\n";
    
    std::cout << "Motor initializion successful." << std::endl;
}

/** @brief Sets pins for pan/tilt servos **/
void init_pan_tilt()
{
    set_pin_input(BCM::Servo_1, m_gpio_mmap);
    set_pin_output(BCM::Servo_1, m_gpio_mmap);

    set_pin_input(BCM::Servo_2, m_gpio_mmap);
    set_pin_output(BCM::Servo_2, m_gpio_mmap);
}

/** @brief Sets pins for all the PWM motor controllers as outputs **/
void init_pwm()
{
    set_pin_input (BCM::MotorPWM_1, m_gpio_mmap);
    set_pin_output(BCM::MotorPWM_1, m_gpio_mmap);
    
    set_pin_input (BCM::MotorPWM_2, m_gpio_mmap);
    set_pin_output(BCM::MotorPWM_2, m_gpio_mmap);
    
    set_pin_input (BCM::MotorPWM_3, m_gpio_mmap);
    set_pin_output(BCM::MotorPWM_3, m_gpio_mmap);
    
    set_pin_input (BCM::MotorPWM_4, m_gpio_mmap);
    set_pin_output(BCM::MotorPWM_4, m_gpio_mmap);
}

/** @brief Sets up motor controller **/
void init_motor_controller()
{
    wiringPiSetup();
    pinMode(BCM::MOTOR_LATCH, OUTPUT);
    pinMode(BCM::MOTOR_DATA,  OUTPUT);
    pinMode(BCM::MOTOR_CLK,   OUTPUT);
}

void write_to_shift_reg(uint8_t latch_data)
{
    digitalWrite(BCM::MOTOR_LATCH, LOW);
    digitalWrite(BCM::MOTOR_DATA, LOW);

    for (int i = 0; i < 8; i++)
    {
        delayMicroseconds(1);

        digitalWrite(BCM::MOTOR_CLK, LOW);

        if (latch_data & (1 << (7 - i)))
        {
            digitalWrite(BCM::MOTOR_DATA, HIGH);
        }
        else
        {
            digitalWrite(BCM::MOTOR_DATA, LOW);
        }

        delayMicroseconds(1);
        digitalWrite(BCM::MOTOR_CLK, HIGH);
    }
        
    digitalWrite(BCM::MOTOR_LATCH, HIGH);
}


}

