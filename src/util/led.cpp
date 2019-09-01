#include <iostream>

#include "led.hpp"

/** @brief Allocates memory for RGB light matrix. */
RGB::RGB()
{
    std::cout << "Initializing LEDs..." << std::endl;

    std::size_t matrix_size = sizeof(ws2811_led_t) * RGB::LedCount;
    ws2811_return_t status;
    void* mem_block;
    
    status = ws2811_init(&m_led_data);
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
        m_led.reset((ws2811_led_t*) mem_block);

    // Setup the configured LED data
    m_led_data.freq   = WS2811_TARGET_FREQ;
    m_led_data.dmanum = 10;

    m_led_data.channel[0].gpionum    = 18;
    m_led_data.channel[0].count      = LedCount;
    m_led_data.channel[0].invert     = 0;
    m_led_data.channel[0].brightness = 255;
    m_led_data.channel[0].strip_type = WS2811_STRIP_RGB;

    m_led_data.channel[1].gpionum    = 0;
    m_led_data.channel[1].count      = 0;
    m_led_data.channel[1].invert     = 0;
    m_led_data.channel[1].brightness = 0;
    
    std::cout << "Led initialization successful." << std::endl;
}
