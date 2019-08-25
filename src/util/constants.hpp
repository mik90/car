#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

namespace Car
{

    // Conventional size of memory block
    const std::size_t BLOCK_SIZE = 4096;

    namespace GPIO
    {
        const unsigned long PI_ZERO_W_BCM2708_PERI_BASE = 0x20000000;
        const unsigned long PI_3_B_BCM2708_PERI_BASE    = 0x3f000000;
        const unsigned long PI_4_B_BCM2708_PERI_BASE    = 0xfe000000;
        const unsigned long GPIO_BASE = PI_3_B_BCM2708_PERI_BASE + 0x200000;
        namespace BCM
        {
            using bcm_pin_t = unsigned int;
            // BCM Pins? Some of these overlap
            const bcm_pin_t beep = 25;
            const bcm_pin_t trig = 25;
            const bcm_pin_t echo = 4;
            const bcm_pin_t servo_1 = 4;
            const bcm_pin_t servo_2 = 25;
            const bcm_pin_t lineTrackLeft = 17;
            const bcm_pin_t lineTrackMiddle = 27;
            const bcm_pin_t lineTrackRight = 22;
        };

        namespace Servos
        {
        };


        namespace Infrared
        {
        };

        namespace Ultrasonic
        {
        };
        
    };


};
#endif