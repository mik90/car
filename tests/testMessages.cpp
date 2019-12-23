#include "gtest/gtest.h"

#include "../common/messages.hpp"

#include <unistd.h>
#include <cerrno>
#include <cstring>


class SerializationTest : public ::testing::Test
{
    private:
    static constexpr int writeIdx = 0;
    static constexpr int readIdx = 0;
    int pipeFd[2];

    protected:
    void SetUp() override
    {
        // Create pipe here
        if (pipe(pipeFd) == -1)
        {
            std::cerr << "Could not create pipe, errno:" << std::strerror(errno) << "\n";
            std::exit(1);
        }
    }

    void TearDown() override
    {
        // Close pipe
        close(pipeFd[writeIdx]);
        close(pipeFd[readIdx]);
    }
};

TEST_F(SerializationTest, PanServoTest)
{
    using namespace motorControllerApi;
    uint8_t buf[messageSize];
    servoControl::serializePanServoAngle(30, buf);

    ASSERT_EQ(servoControl::deserializePanServoAngle(buf), 30);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}