#ifndef WIRING_PI_MOCKS_HPP_
#define WIRING_PI_MOCKS_HPP_

#include <memory>
#include "common_rpi.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

class WiringPiInterface
{
    public:
        virtual void wiringPiSetup()       = 0;
        virtual void pinMode(int, int)     = 0;
        virtual void digitalWrite(int,int) = 0;
        virtual void delayMicroSeconds(unsigned int) = 0;
        virtual void setPinInput(const Car::bcm_pin_t,
                 const std::shared_ptr<volatile uint32_t>&) = 0;
        virtual void setPinOutput(const Car::bcm_pin_t,
                 const std::shared_ptr<volatile uint32_t>&) = 0;
        virtual ~WiringPiInterface();
};

class WiringPiMocks : public WiringPiInterface
{
    public:
        virtual ~WiringPiMocks() {}
        MOCK_METHOD0(wiringPiSetup, void());
        MOCK_METHOD1(pinMode, void(int, int));
        MOCK_METHOD2(digitalWrite, void(int, int));
        MOCK_METHOD3(delayMicroSeconds, void(unsigned int));
        MOCK_METHOD4(setPinInput, void(const Car::bcm_pin_t,
                    const std::shared_ptr<volatile uint32_t>&));
        MOCK_METHOD5(setPinOutput, void(const Car::bcm_pin_t,
                    const std::shared_ptr<volatile uint32_t>&));

};

#endif