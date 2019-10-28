#ifndef IR_REMOTE_HPP_
#define IR_REMOTE_HPP_

#include <bitset>

namespace Car
{

namespace IrRemoteCommands
{
    // Wheel controls
    constexpr uint16_t GoForward {0x46b9};
    constexpr uint16_t GoBackward{0x15ea};

    constexpr uint16_t TurnLeft {0x44bb};
    constexpr uint16_t TurnRight{0x43bc};

    constexpr uint16_t Stop {0x40bf};

    constexpr uint16_t IncreaseWheelSpeed{0x16e9};
    constexpr uint16_t DecreaseWheelSpeed{0x19e6};

    // Camera controls
    constexpr uint16_t TiltUp  {0x0cf3};
    constexpr uint16_t TiltDown{0x18e7};

    constexpr uint16_t PanLeft {0x08f7};
    constexpr uint16_t PanRight{0x1ce3};

    // Track mode
    constexpr uint16_t EnterTrackMode{0x42bd};
    constexpr uint16_t ExitTrackMode {0x4ab5};
}


}

#endif