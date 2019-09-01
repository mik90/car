
#include <gtest/gtest.h>

namespace
{

class InitTest : public ::testing::Test
{
    protected:
};

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}