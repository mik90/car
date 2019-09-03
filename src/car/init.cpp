
#include <cstring>
#include <iostream>

#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "wiringPi.h"

#include "car.hpp"
#include "common_rpi.hpp"

namespace Car
{

/** @brief Custom deleter. Unmaps the mapped memory pointer to by
 *  gpioMmap_ptr
 *  @param[in] gpioMmap - Pointer to base of memory-mapped GPIO **/
void delete_GPIO_map(volatile uint32_t* gpioMmap_ptr)
{
    if (munmap((void*) gpioMmap_ptr, BLOCK_SIZE) != 0)
    {
        std::cerr << "Cannot unmap GPIO map. errno:" << errno << '\n';
    }
}

/** @brief Inits pointer to a shared memory-mapped space used
 *  for general purpose Input/output. Assigns custom delter to the
 *  shared pointer.
 *  @note Only works on target (Raspberry PI 3)  **/
Car::Car() : m_gpioMmap(nullptr, delete_GPIO_map)
{
    std::cout << "Initializing sensors..." << std::endl;
    void* gpioMmap = nullptr;

    int dev_mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (dev_mem_fd < 0)
    {
        std::cerr << "open(/dev/mem) failure. errorno: " << std::strerror(errno) << '\n';
        m_gpioMmap = nullptr;
        return;
    }

    gpioMmap = mmap(NULL,                   // Kernel can use whatever address
                    BLOCK_SIZE,             // Length of memory section
                    PROT_READ | PROT_WRITE, // Enable R/W
                    MAP_SHARED,             // For use by other processes
                    dev_mem_fd,             // We are mapping /dev/mem to this
                    GPIO::GPIO_BASE);       // Base offset for GPIO
    // Done mapping the memory
    close(dev_mem_fd);

    if (gpioMmap == MAP_FAILED)
    {
        std::cerr << "mmap() failure. errorno: " << std::strerror(errno) << '\n';
        m_gpioMmap = nullptr;
        return;
    }
    // Cast the gpio pointer to a volatile uint32_t
    // Car's shared ptr now points to the memory mapped GPIO
    m_gpioMmap.reset(static_cast<volatile uint32_t*>(gpioMmap));

    // Copy the shared pointer over to the sensor and motor managers
    m_sensors.setMemoryMap(m_gpioMmap); 
    m_motors.setMemoryMap(m_gpioMmap); 

    this->initBeep();
    std::cout << "Beeper is initialized" << std::endl;
}



/** @brief Sets pin for the beeper as output and writes it low **/
void Car::initBeep()
{
    pinMode(BCM::Beep, OUTPUT);
    digitalWrite(BCM::Beep, LOW);
}

}