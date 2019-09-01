#ifndef MSG_HPP_
#define MSG_HPP_

#include <cstdint>

namespace Car
{
enum class Direction_t : std::uint8_t {FORWARD       = 0,
                                       FORWARD_RIGHT = 1,
                                       FORWARD_LEFT  = 2,
                                       REVERSE       = 3,
                                       REVERSE_LEFT  = 4,
                                       REVERSE_RIGHT = 5,
                                       LEFT          = 6,
                                       RIGHT         = 7};

enum class Acceleration_t : std::uint8_t  {ZERO = 0,
                                           HALF = 50,
                                           FULL = 100};
struct MsgMove_t
{
    Direction_t    dir;
    Acceleration_t throttle;
} __attribute__((packed));

}

#endif