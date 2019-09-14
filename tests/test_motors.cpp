#include "motors.hpp"
#include "wiringPiMocks.hpp"
#include "gtest/gtest.h"

WiringPiMocks wiringPiMocks;

namespace
{

class MotorTest : public ::testing::Test
{
    MotorTest() {}

    ~MotorTest() override {}
    
};

TEST_F(MotorTest, MoveCarFOrward)
{
    Car::Motors m;
}

}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}