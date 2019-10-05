#include "common_rpi.hpp"

namespace Car
{

RpiInterface::RpiInterface()
{
    std::cout << "Initializing Rpi interface..." << std::endl;
    void* gpioMmapPtr = nullptr;

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

// Attempt to refactor INP_GPIO, OUT_GPIO and other weird macros that seem to be
// all over the pi/arduino world
void RpiInterface::setPinInput(const pin_t pin)
{
    auto adjusted_ptr = (m_gpioMmapPtr.get() + ((pin) / 10));
    auto adjusted_pin = 7 << (((pin) % 10 ) * 3);
    *adjusted_ptr &= ~(adjusted_pin);
}

void RpiInterface::setPinOutput(const pin_t pin)
{
    auto adjusted_ptr = (m_gpioMmapPtr.get() + ((pin) / 10));
    auto adjusted_pin = 1 << (((pin) % 10 ) * 3);
    *adjusted_ptr |= adjusted_pin;
}

/** @brief Writes to the GPIO Pull up/down controller */
void RpiInterface::writeToPullUpDown(uint32_t data)
{
    *(m_gpioMmapPtr.get() + 37) = data;
}

/** @brief Writes to the GPIO Pull up/down clock */
void RpiInterface::writeToPullUpDownClock(uint32_t data)
{
    *(m_gpioMmapPtr.get() + 38) = data;
}

/** @brief Sets bits which are 1, ignores bits tha are 0
 *  @param[in] bits - Bitset where some bits are 1  **/
void RpiInterface::writeGpioBits(std::bitset<32> bits)
{
    *(m_gpioMmapPtr.get() + 7) = bits.to_ulong();
}

/** @brief clears bits which are 1, ignores bits that are 0.
 *  @param[in] bits - Bitset where some bits are 1  **/
void RpiInterface::clearGpioBits(std::bitset<32> bits)
{
    *(m_gpioMmapPtr.get() + 10) = bits.to_ulong();
}


}