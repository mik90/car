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
[[nodiscard]] volatile unsigned int* Sensors::init_GPIO()
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

/** @brief Unmaps the mapped memory **/
void Sensors::cleanup_GPIO(void* gpio_map)
{
    if (munmap(gpio_map, BLOCK_SIZE) != 0)
    {
        std::cerr << "Cannot unmap memory. errno:" << errno << std::endl;
    }
}


};