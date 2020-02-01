#include <iostream>
#include <string>
#include <thread>

#include "../car.hpp"


void runFaceDetectorLoop()
{
    using namespace Car;
    using namespace std::chrono_literals;

    Car::Car c;
    std::cout << "Starting face detector loop..." << std::endl;

    while(true)
    {
        if (c.getFaceDetector().areFacesAndEyesVisible() == true)
        {
            std::cout << "I saw a face!" << std::endl;
            c.setSpeed(50);
            c.moveCar(CarMovement_t::FORWARD);
            std::this_thread::sleep_for(1s);
            c.moveCar(CarMovement_t::STOP);
        }
    }
}

int main(int argc, char** argv)
{
    runFaceDetectorLoop();
    return 0;
}