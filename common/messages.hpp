#ifndef MESSAGES_HPP
#define MESSAGES_HPP

// This is the interface between Arduino's motorController and Rpi's carController
// It needs to be able to be parsed by Arduino's compiler setup and GCC

// AVR is little endian and ARM's default endianness is also little endian.
// ARM can switch between little and big endian, but for this purpose, I won't
// convert anything to network-byte order, which is big endian, or a native-byte
// order since they should both be little endian.

#ifndef ARDUINO
// For building with GCC, we need cstdint for uint_8
#include <cstdint>
#include <algorithm> // Brings in min/max
using std::min;
using std::max;
#endif

namespace motorControllerApi
{
constexpr size_t messageSize = 4;
constexpr uint8_t InvalidServoAngle = 255;
constexpr uint8_t InvalidWheelSpeed = 255;
constexpr uint16_t maxDistance_cm = 65535;
constexpr uint32_t baudRate = 9600;

inline uint32_t operator "" _uint32_t(unsigned long long value)
{
    return static_cast<uint32_t>(value);
}
inline uint8_t operator "" _uint8_t(unsigned long long value)
{
    return static_cast<uint8_t>(value);
}

enum class MessageId_t : uint8_t {SERVO_CONTROL   = 0,
                                  WHEEL_CONTROL   = 1,
                                  ULTRASONIC_INFO = 2};

// Need the "M_" since macros will overwrite these otherwise
// These values are defined in Adafruit_MotorShield.h, i just didn't
// want to use more macros
enum class MotorDir_t : uint8_t {M_INVALID_DIR = 0,
                                 M_FORWARD     = 1,
                                 M_REVERSE     = 2,
                                 M_BRAKE       = 3,
                                 M_RELEASE     = 4};

// Pi -> Arduino --------------------
namespace servoControl
{
    static constexpr uint8_t id = static_cast<uint8_t>(MessageId_t::SERVO_CONTROL);

    inline void serializePanServoAngle(uint8_t panServoAngle, uint8_t* outputBuf)
    {
        outputBuf[0] = id;
        // Cap it at 200
        if (panServoAngle > 200)
            panServoAngle = 200;
        outputBuf[1] = panServoAngle;
        outputBuf[2] = InvalidServoAngle;
        outputBuf[3] = 0x00; // Padding
    }
    
    inline void serializeTiltServoAngle(uint8_t tiltServoAngle, uint8_t* outputBuf)
    {
        outputBuf[0] = id;
        outputBuf[1] = InvalidServoAngle;
        // Cap it at 200
        if (tiltServoAngle > 200)
            tiltServoAngle = 200;
        outputBuf[2] = tiltServoAngle;
        outputBuf[3] = 0x00; // Padding
    }

    // ---
    inline uint8_t deserializePanServoAngle(const uint8_t* msg)
    {
        return msg[1];
    }

    inline uint8_t deserializeTiltServoAngle(const uint8_t* msg)
    {
        return msg[2];
    }
};

namespace wheelControl
{
    static constexpr uint8_t id = static_cast<uint8_t>(MessageId_t::WHEEL_CONTROL);

    inline void serializeWheelSpeed(uint8_t wheelSpeed, uint8_t* outputBuf)
    {
        // Speed range is 0-255
        wheelSpeed = min(wheelSpeed, InvalidWheelSpeed);
        outputBuf[0] = id;
        outputBuf[1] = wheelSpeed;
        outputBuf[3] = static_cast<uint8_t>(MotorDir_t::M_INVALID_DIR);
        outputBuf[4] = static_cast<uint8_t>(MotorDir_t::M_INVALID_DIR);
    }

    inline void serializeWheelDirs(MotorDir_t leftSideDir, MotorDir_t rightSideDir, uint8_t* outputBuf)
    {
        outputBuf[0] = id;
        outputBuf[1] = InvalidWheelSpeed;
        outputBuf[2] = static_cast<uint8_t>(leftSideDir);
        outputBuf[3] = static_cast<uint8_t>(rightSideDir);
    }
    // ---
    inline uint8_t deserializeWheelSpeed(const uint8_t* msg)
    {
        // Return the wheel speed
        return msg[1];
    }

    inline void deserializeWheelDirs(const uint8_t* msg,
                                     MotorDir_t* outLeftDir, MotorDir_t* outRightDir)
    {

        *outLeftDir  = static_cast<MotorDir_t>(msg[2]);
        *outRightDir = static_cast<MotorDir_t>(msg[3]);
    }
};


// Arduino -> Pi --------------------
namespace ultrasonicInfo
{
    static constexpr uint8_t id = static_cast<uint8_t>(MessageId_t::ULTRASONIC_INFO);

    inline void serializeDistance(uint32_t distance_cm, uint8_t* outputBuf)
    {
        // Clamp it to uint16 max
        uint16_t distanceClamped = min(65535_uint32_t, distance_cm);
        // Serialize into bytes
        uint8_t distanceUpper = distanceClamped >> 1; 
        uint8_t distanceLower = distanceClamped & 0x00ff; 
        outputBuf[0] = id;
        outputBuf[1] = 0x00;
        outputBuf[2] = distanceUpper;
        outputBuf[3] = distanceLower;
    }
    // ---
    inline uint16_t deserializeDistance(const uint8_t* msg)
    {
        // Shift left by a byte
        uint16_t distanceUpper = static_cast<uint16_t>(msg[2]) << 1;
        uint16_t distanceLower = static_cast<uint16_t>(msg[3]);
        return distanceUpper | distanceLower;
    }
};

}
#endif