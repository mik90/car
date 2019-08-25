#include <sys/types.h>

#include <cstring>

#include "sensors.hpp"
#include "constants.hpp"

namespace Car
{

/** @brief Grabs pointer to a shared memory-mapped space used
 *  for general purpose Input/output
 *  @note Only works on target (Raspberry PI 3)
 * 
 *  @return pointer to memory mapped area    **/
[[nodiscard]] volatile unsigned int* Sensors::init_GPIO(void)
{
    void* gpio_map = nullptr;

    int dev_mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (dev_mem_fd < 0)
    {
        std::cerr << "open(/dev/mem) error: " << std::strerror(errno) << '\n';
        return nullptr;
    }

    gpio_map = mmap(NULL,                   // Kernel can use whatever address
                    BLOCK_SIZE,             // Length of memory section
                    PROT_READ | PROT_WRITE, // Enable R/W
                    MAP_SHARED,             // For use by other processes
                    dev_mem_fd,             // We are mapping /dev/mem to this
                    GPIO::GPIO_BASE);       // Base offset for GPIO
    // Done mapping the memory
    close(dev_mem_fd);

    if (gpio_map == MAP_FAILED)
    {
        std::cerr << "mmap() error: " << std::strerror(errno) << '\n';
        return nullptr;
    }

    return (volatile unsigned int*) gpio_map;
}

/** @brief Unmaps the mapped memory. Although passed in as volatile unsigned
 *  int*, it is cast to void*
 *  @param[out] gpio_mmap - Pointer to base of memory-mapped GPIO **/
void Sensors::cleanup_GPIO(volatile unsigned int* gpio_mmap)
{
    if (munmap((void*) gpio_mmap, BLOCK_SIZE) != 0)
    {
        std::cerr << "Cannot unmap memory. errno:" << errno << std::endl;
    }
}

/** @brief Sets ultrasonic sensor pins **/
void Sensors::init_ultrasonic(void)
{
    // This needs to be more clear than "Echo" and "Trig"
    pinMode(BCM::Echo, INPUT);
    pinMode(BCM::Trig, OUTPUT);
}

/** @brief Sets pins for pan/tilt servos
 *  @param[out] gpio_mmap - Pointer to base of memory-mapped GPIO **/
void Sensors::init_pan_tilt(volatile unsigned int* gpio_mmap)
{
    set_pin_input(BCM::Servo_1, gpio_mmap);
    set_pin_output(BCM::Servo_1, gpio_mmap);

    set_pin_input(BCM::Servo_2, gpio_mmap);
    set_pin_output(BCM::Servo_2, gpio_mmap);
}

/** @brief Sets pins for line reader sensors
 *  @param[out] gpio_mmap - Pointer to base of memory-mapped GPIO **/
void Sensors::init_line_reader(volatile unsigned int* gpio_mmap)
{
    volatile unsigned int* gpio_pull_ptr = get_gpio_pull_ptr(gpio_mmap);

    set_pin_input(BCM::LineTrackLeft, gpio_mmap);
    *gpio_pull_ptr = 1 << BCM::LineTrackLeft;

    set_pin_input(BCM::LineTrackMiddle, gpio_mmap);
    *gpio_pull_ptr = 1 << BCM::LineTrackMiddle;

    set_pin_input(BCM::LineTrackRight, gpio_mmap);
    *gpio_pull_ptr = 1 << BCM::LineTrackRight;
}

/** @brief Sets pin for the beeper as output and writes it low **/
void Sensors::init_beep(void)
{
    pinMode(BCM::Beep, OUTPUT);
    digitalWrite(BCM::Beep, LOW);
}

/** @brief This was originally countLow() in remoteserver.c but it doesn't
 *  seem to make much sense **/
unsigned char count_infrared(void)
{
    unsigned char count = 0;
    while (digitalRead(BCM::InfraredIn) == 0)
    {
        // Wait for it to go high? I thought this was reading low...
    }

    while (digitalRead(BCM::InfraredIn) == 1)
    {
        count++;
        // 26 seems arbitrary, why was this chosen?
        // I guess it's just incrementing the count every 26 microseconds
        // while the signal is high
        delayMicroseconds(26);
    }

    return count;
}

// TODO These need to be cleaned up. I can't even find where they're
// used in the old remoteserver.c file
// Bits is a global variable in the original code
unsigned char bits = 0;
// Buffer is a global variable in the original code
unsigned char buffer[Sensors::InfraredBufferLimit];

bool infrared_ISR_done = false;
/** @brief Interrupt service routine for the infrared sensor.
 *  Triggered on the falling edge.
 *  @note I really doubt this code actually does anything in the
 *  old repo **/
void infrared_ISR(void)
{
    const unsigned char BitsPerByte = 8;
    const unsigned char initialCount = count_infrared();
    unsigned char count = 0;
    bits = 0;

    for (unsigned char i = 0; i < Sensors::InfraredBufferLimit; i++)
    {
        for (unsigned char j = 0; j < BitsPerByte; j++)
        {
            count = count_infrared();
            if (count == 0)
            {
                buffer[i] >>= (8 - j);
                infrared_ISR_done = true;
                return;
            }

            buffer[i] >>= 1;

            // Once again, arbitrary number
            // Look at the original code for this though, it's hilarious
            if (count > 30)
                buffer[i] += 0x80;

            bits++;
        }
    }
    infrared_ISR_done = true;
}

/** @brief Sets pin for the infrared sensor as input. Sets the pull
 * up/down resistor as up, and sets ISR for falling edge. **/
void Sensors::init_infrared(void)
{
    pinMode(BCM::InfraredIn, INPUT);
    pullUpDnControl(BCM::InfraredIn, PUD_UP);
    // TODO This ISR can't even be doing much, maybe we don't need it.
    wiringPiISR(BCM::InfraredIn, INT_EDGE_FALLING, infrared_ISR);
}

/** @brief Sets pins for all the PWM motor controllers as outputs
 *  @param[out] gpio_mmap - Pointer to base of memory-mapped GPIO **/
void Sensors::init_pwm(volatile unsigned int* gpio_mmap)
{
    set_pin_input (BCM::MotorPWM_1, gpio_mmap);
    set_pin_output(BCM::MotorPWM_1, gpio_mmap);
    
    set_pin_input (BCM::MotorPWM_2, gpio_mmap);
    set_pin_output(BCM::MotorPWM_2, gpio_mmap);
    
    set_pin_input (BCM::MotorPWM_3, gpio_mmap);
    set_pin_output(BCM::MotorPWM_3, gpio_mmap);
    
    set_pin_input (BCM::MotorPWM_4, gpio_mmap);
    set_pin_output(BCM::MotorPWM_4, gpio_mmap);
}


/** @brief Sets matrix for RGB lights. Technically not sensors, but
 *  the commands are similar enough.
 *  @param[out] ws2811_led_t - Pointer RGB matrix **/
void RGB::init_rgb(ws2811_led_t* led_ptr)
{
    std::size_t matrix_size = sizeof(ws2811_led_t) * RGB::LedCount;
    ws2811_return_t status;
    void* mem_block;
    
    status = ws2811_init(&led_data);
    if(status != WS2811_SUCCESS)
    {

        std::cerr << "ws2811_init failed:" << ws2811_get_return_t_str(status) << std::endl;
        return;
    }

    // Allocate a block of memory but don't throw an exception
    mem_block = new (std::nothrow) uint8_t[matrix_size];

    if (mem_block == nullptr)
        std::cerr << "Allocation of LED matrix failed\n";
    else
        // Copy over pointer due to success
        led_ptr = (ws2811_led_t*) mem_block;
}


};