#include "irRemote.hpp"
#include "commonRpi.hpp"

#include "wiringPi.h"


namespace Car
{


static constexpr std::size_t IrBufferLimit{64};
// So, it seems to only use buffer indices 2 and 3
// 0 and 1 are just headers? everything else can be ignored?
static unsigned char buffer[IrBufferLimit];

uint16_t getIrCommand()
{
    // buffer 2 is the upper byte, buffer 3 is the lower byte
    uint16_t irCommand{0};
    irCommand = buffer[2];
    irCommand <<= 8;
    irCommand |= buffer[3];
    return irCommand; 
}

unsigned int countLow()
{
    unsigned int timeoutCounter{0};
    static constexpr unsigned int timeout{250};

    while(digitalRead(wPiPins::InfraredIn) == 0)
    {
        // Wait until pin is 1? although wouldn't it be high?
    }
    
    // TODO this is still unclean
    // Arbitrary timeout
    while(digitalRead(wPiPins::InfraredIn) == 1 && timeoutCounter < timeout)
    {
        // count while pin is high?
        timeoutCounter++;
        delayMicroseconds(26);
    }

    if (timeoutCounter == 250)
        std::cerr << "DEBUG: IR remote hit timeout counter\n";
    return timeoutCounter;
}

// This should be a wiringPi ISR, called whenever the IR
// signal goes LOW
void infraredIsr()
{
    uint32_t bits{0};

    // I'm guessing there's some sort of circular buffer of input
    // or something

    for (std::size_t bufferIndex = 0; bufferIndex < IrBufferLimit; bufferIndex++)
    {
        // For each buffer index (64)

        for (std::size_t bitNum = 0; bitNum < 8; bitNum++)
        {
            // For each bit in each byte of the buffer
            auto irCount{countLow()};
            if (irCount == 0)
            {
                // Seems like, once an irCount of 0 is hit, the data is
                // zero'd out and the ISR is done

                // The higher the bit number, the more rightward
                // it shifts
                buffer[bufferIndex] >>= (8 - bitNum);
                // TODO Done? wtf, does this zero out the data?
                return;
            }

            // For each bit, shift it to the right once done
            buffer[bufferIndex] >>= 1;

            if (irCount > 30)
            {
                // wtf? cap it out at 30 for each bit?
                // 0x80 == 128 == 0b1000 0000
                buffer[bufferIndex] += 0x80;
            }

            // Total number of bits in the buffer? this wasn't
            // even used
            bits++;

        }

    }
}


};