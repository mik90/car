#include "gtest/gtest.h"

#include "../common/messages.hpp"


class MessageTestFixture : public ::testing::Test
{
    // Currently have nothing in here yet
};

TEST_F(MessageTestFixture, PanServoMessage)
{
    using namespace motorControllerApi;
    uint8_t buf[messageSize];
    servoControl::serializePanServoAngle(30, buf);

    ASSERT_EQ(30, servoControl::deserializePanServoAngle(buf));
}

TEST_F(MessageTestFixture, TiltServoMessage)
{
    using namespace motorControllerApi;
    uint8_t buf[messageSize];
    servoControl::serializeTiltServoAngle(25, buf);

    ASSERT_EQ(25, servoControl::deserializeTiltServoAngle(buf));
}

TEST_F(MessageTestFixture, WheelSpeedMessage)
{
    using namespace motorControllerApi;
    uint8_t buf[messageSize];
    wheelControl::serializeWheelSpeed(75, buf);

    ASSERT_EQ(75, wheelControl::deserializeWheelSpeed(buf));
}

TEST_F(MessageTestFixture, WheelDirsMessage)
{
    using namespace motorControllerApi;
    uint8_t buf[messageSize];
    wheelControl::serializeWheelDirs(MotorDir_t::M_FORWARD,
                                     MotorDir_t::M_REVERSE,
                                     buf);

    MotorDir_t leftSideDir = MotorDir_t::M_INVALID_DIR;
    MotorDir_t rightSideDir = MotorDir_t::M_INVALID_DIR;
    wheelControl::deserializeWheelDirs(buf, &leftSideDir, &rightSideDir);
    ASSERT_EQ(MotorDir_t::M_FORWARD, leftSideDir);
    ASSERT_EQ(MotorDir_t::M_REVERSE, rightSideDir);
}

TEST_F(MessageTestFixture, distanceMessage)
{
    using namespace motorControllerApi;
    uint8_t buf[messageSize];
    ultrasonicInfo::serializeDistance(15, buf);

    ASSERT_EQ(15, ultrasonicInfo::deserializeDistance(buf));
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}