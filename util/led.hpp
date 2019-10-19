#ifndef LED_HPP_
#define LED_HPP_

#include <memory>

#include "ws2811.h"

namespace Car
{
    /** @brief Controls the LED lights **/
    class RGB
    {
        private:
            // 4 LEDs wide
            const unsigned int LedRows = 4;
            // 4 LEDs tall 
            const unsigned int LedColumns = 4;
            const unsigned int LedCount = LedRows * LedColumns;

            std::unique_ptr<ws2811_led_t> m_led;
            ws2811_t m_ledData;
        public:
            // Allocates memory for LED matrix
            RGB();
    };

}

#endif