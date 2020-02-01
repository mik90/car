#include "gtest/gtest.h"

#include "../common/messages.hpp"


class MessageTestFixture : public ::testing::Test
{
    // Currently have nothing in here yet
};

using namespace msg;

TEST_F(MessageTestFixture, PanServoMessage)
{
    uint8_t buf[messageSize];
    serializePanServoAngle(30, buf);

    ASSERT_EQ(30, deserializePanServoAngle(buf));
}

TEST_F(MessageTestFixture, TiltServoMessage)
{
    uint8_t buf[messageSize];
    serializeTiltServoAngle(25, buf);

    ASSERT_EQ(25, deserializeTiltServoAngle(buf));
}

TEST_F(MessageTestFixture, WheelSpeedMessage)
{
    uint8_t buf[messageSize];
    serializeWheelSpeed(75, buf);

    ASSERT_EQ(75, deserializeWheelSpeed(buf));
}

TEST_F(MessageTestFixture, WheelDirsMessage)
{
    uint8_t buf[messageSize];
    serializeWheelDirs(MotorDir_t::M_FORWARD,
                                     MotorDir_t::M_REVERSE,
                                     buf);

    MotorDir_t leftSideDir = MotorDir_t::M_INVALID_DIR;
    MotorDir_t rightSideDir = MotorDir_t::M_INVALID_DIR;
    deserializeWheelDirs(buf, &leftSideDir, &rightSideDir);
    ASSERT_EQ(MotorDir_t::M_FORWARD, leftSideDir);
    ASSERT_EQ(MotorDir_t::M_REVERSE, rightSideDir);
}

TEST_F(MessageTestFixture, distanceMessage)
{
    uint8_t buf[messageSize];
    serializeDistance(15, buf);

    ASSERT_EQ(15, deserializeDistance(buf));
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}