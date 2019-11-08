#include "commonRpi.hpp"

namespace Car
{

RpiInterface::RpiInterface()
{
    std::cout << "Initializing Rpi interface..." << std::endl;
    void* gpioMmapPtr{nullptr};

    // TODO Just open /dev/gpiomem as it does not require more permissions
    // Better yet, does this even need to be mapped to memory?
    int dev_mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (dev_mem_fd < 0)
    {
        std::cerr << "open(/dev/mem) failure. errorno: " << std::strerror(errno) << '\n';
        gpioMmapPtr = nullptr;
        return;
    }

    gpioMmapPtr = mmap(NULL,                   // Kernel can use whatever address
                       BLOCK_SIZE,             // Length of memory section
                       PROT_READ | PROT_WRITE, // Enable R/W
                       MAP_SHARED,             // For use by other processes
                       dev_mem_fd,             // We are mapping /dev/mem to this
                       GPIO::GPIO_BASE);       // Base offset for GPIO
    // Done mapping the memory
    close(dev_mem_fd);

    if (gpioMmapPtr == MAP_FAILED)
    {
        std::cerr << "mmap() failure. errorno: " << std::strerror(errno) << '\n';
        gpioMmapPtr = nullptr;
        return;
    }

    // Initialize the wiringPi library
    wiringPiSetup();

    // Set the unique_ptr to a volatile u32 with a custom deleter
    // Cast the void pointer to a volatile u32 pointer
    m_gpioMmapPtr.reset(static_cast<volatile uint32_t*>(gpioMmapPtr));
}

/** @brief Writes to the GPIO Pull up/down controller */
void RpiInterface::writeToPullUpDown(uint32_t data)
{
    *(m_gpioMmapPtr.get() + 37) = data;
}

}