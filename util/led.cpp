#include <iostream>

#include "led.hpp"

namespace Car
{

/** @brief Allocates memory for RGB light matrix. */
RGB::RGB()
{
    std::cout << "Initializing LEDs...\n";

    std::size_t matrixSize = sizeof(ws2811_led_t) * RGB::LedCount;
    ws2811_return_t status;
    void* memBlock;
    
    status = ws2811_init(&m_ledData);
    if(status != WS2811_SUCCESS)
    {
        std::cerr << "ws2811_init failed. ws2811 status:" << ws2811_get_return_t_str(status) << "\n";
        return;
    }

    // Allocate a block of memory but don't throw an exception
    memBlock = new (std::nothrow) uint8_t[matrixSize];

    if (memBlock == nullptr)
    {
        std::cerr << "Allocation of LED matrix failed\n";
        return;
    }
    else
    {
        // Copy over pointer due to success
        m_led.reset((ws2811_led_t*) memBlock);
    }

    // Setup the configured LED data
    m_ledData.freq   = WS2811_TARGET_FREQ;
    m_ledData.dmanum = 10;

    m_ledData.channel[0].gpionum    = 18;
    m_ledData.channel[0].count      = LedCount;
    m_ledData.channel[0].invert     = 0;
    m_ledData.channel[0].brightness = 255;
    m_ledData.channel[0].strip_type = WS2811_STRIP_RGB;

    m_ledData.channel[1].gpionum    = 0;
    m_ledData.channel[1].count      = 0;
    m_ledData.channel[1].invert     = 0;
    m_ledData.channel[1].brightness = 0;
    
    std::cout << "Led initialization successful." << std::endl;
}

}