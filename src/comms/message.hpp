#ifndef MSG_HPP_
#define MSG_HPP_

#include <cstdint>

namespace Car
{
enum class Direction_t : unsigned int {FORWARD = 0x00,
                                        REVERSE = 0x01,
                                        LEFT    = 0x10,
                                        RIGHT   = 0x20};

enum class Acceleration_t : unsigned int  {ZERO = 0,
                                           HALF = 50,
                                           FULL = 100};
struct MsgMove_t
{
    Direction_t    dir;
    Acceleration_t throttle;
} __attribute__((packed));

}

#endif