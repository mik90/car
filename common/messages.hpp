#ifndef MESSAGES_HPP
#define MESSAGES_HPP

// This is the interface between Arduino's motorController and Rpi's carController

#ifndef ARDUINO
// Arduino has these defined natively
#include <cstdint>
#else
#include <stdint.h>
#endif

#include <algorithm>
#include <array>
#include <limits>
#include <utility>

namespace motorControllerApi
{
constexpr uint8_t messageSize = 4;
constexpr uint8_t InvalidServoAngle = 101;
constexpr uint8_t InvalidWheelSpeed = 255;
constexpr uint16_t maxDistance_cm = std::numeric_limits<uint16_t>::max();
constexpr uint32_t baudRate = 9600;

inline uint32_t operator "" _u32(unsigned long long value)
{
    return static_cast<uint32_t>(value);
}
inline uint8_t operator "" _u8(unsigned long long value)
{
    return static_cast<uint8_t>(value);
}

enum class MessageId_t : uint8_t {SERVO_CONTROL   = 0,
                                  WHEEL_CONTROL   = 1,
                                  ULTRASONIC_INFO = 2};
enum class MotorDir_t : uint8_t {INVALID_DIR = 0,
                                 FORWARD     = 1,
                                 REVERSE     = 2,
                                 RELEASE     = 3};

// Pi -> Arduino --------------------
namespace servoControl
{
    static constexpr uint8_t id = static_cast<uint8_t>(MessageId_t::SERVO_CONTROL);

    inline std::array<uint8_t,messageSize>
    serializePanServoAngle(uint8_t panServoAngle)
    {
        // Range is 0-100
        panServoAngle = std::min(panServoAngle, 100_u8);
        return std::array<uint8_t, messageSize>{id,
                                                panServoAngle,
                                                InvalidServoAngle,
                                                0x00}; // Padding
    }
    
    inline std::array<uint8_t,messageSize>
    serializeTiltServoAngle(uint8_t tiltServoAngle)
    {
        // Range is 0-100
        tiltServoAngle = std::min(tiltServoAngle, 100_u8);
        return std::array<uint8_t, messageSize>{id, 
                                                InvalidServoAngle,
                                                tiltServoAngle,
                                                0_u8}; // Padding
    }

    // ---
    inline uint8_t
    deserializePanServoAngle(const std::array<uint8_t,messageSize>& buf)
    {
        return buf[1];
    }

    inline uint8_t
    deserializeTiltServoAngle(const std::array<uint8_t,messageSize>& buf)
    {
        return buf[2];
    }
};

namespace wheelControl
{
    static constexpr uint8_t id = static_cast<uint8_t>(MessageId_t::WHEEL_CONTROL);

    inline std::array<uint8_t,messageSize>
    serializeWheelSpeed(uint8_t wheelSpeed)
    {
        // Speed range is 0-255
        wheelSpeed = std::min(wheelSpeed, InvalidWheelSpeed);
        return std::array<uint8_t, messageSize>{id,
                                                wheelSpeed,
                                                static_cast<uint8_t>(MotorDir_t::INVALID_DIR),
                                                static_cast<uint8_t>(MotorDir_t::INVALID_DIR)};
    }

    inline std::array<uint8_t,messageSize>
    serializeWheelDirs(MotorDir_t leftSideDir, MotorDir_t rightSideDir)
    {
        return std::array<uint8_t, messageSize>{id,
                                                InvalidWheelSpeed,
                                                static_cast<uint8_t>(leftSideDir),
                                                static_cast<uint8_t>(rightSideDir)};
    }
    // ---
    inline uint8_t
    deserializeWheelSpeed(const std::array<uint8_t,messageSize>& buf)
    {
        return buf[1];
    }

    inline std::pair<MotorDir_t, MotorDir_t>
    deserializeWheelDirs(const std::array<uint8_t,messageSize>& buf)
    {
        return std::make_pair(static_cast<MotorDir_t>(buf[2]),
                              static_cast<MotorDir_t>(buf[3]));
    }
};


// Arduino -> Pi --------------------
namespace ultrasonicInfo
{
    static constexpr uint8_t id = static_cast<uint8_t>(MessageId_t::ULTRASONIC_INFO);

    inline std::array<uint8_t,messageSize>
    serializeDistance(uint32_t distance_cm)
    {
        // Clamp it to uint16 max
        uint16_t distanceClamped = std::min(65535_u32, distance_cm);
        // Serialize into bytes
        uint8_t distanceUpper = distanceClamped >> 1; 
        uint8_t distanceLower = distanceClamped & 0x00ff; 
        return std::array<uint8_t,4>{id,
                                     0_u8, // Padding
                                     distanceUpper,
                                     distanceLower};

    }
    // ---
    inline uint16_t 
    deserializeDistance(const std::array<uint8_t, messageSize>& buf)
    {
        // Shift left by a byte
        uint16_t distanceUpper = static_cast<uint16_t>(buf[2]) << 1;
        uint16_t distanceLower = static_cast<uint16_t>(buf[3]);
        return distanceUpper | distanceLower;
    }
};

}
#endif