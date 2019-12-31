#include <iostream>
#include <string>
#include <thread>

#include "car.hpp"


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

        std::this_thread::sleep_for(2s);
    }
}

int main(int argc, char** argv)
{
    runFaceDetectorLoop();
    return 0;
}

// Unused
void runCliLoop()
{
    using namespace Car;
    using namespace std::chrono_literals;

    Car::Car c;
    std::cout << "Starting CLI loop..." << std::endl;

    std::string input;
    while (true)
    {
        std::cin >> input;

        // Big ol if/else so i can test each motor
        if (input == "end")
        {
            std::cout << "Returning..." << std::endl;
            c.moveCar(CarMovement_t::STOP);
            return;
        }
        else if (input == "pan")
        {
            // pan <target_value>
            std::cout << "panning..." << std::endl;
            std::cin >> input;
            std::string::size_type strSize{0};
            uint8_t value = std::stoi(input, &strSize);
            if  (strSize > 0)
                c.pan(value);
            else
                std::cerr << "Could not parse into unsigned int:" << input << std::endl;
        }
        else if (input.find("tilt") != std::string::npos)
        {
            // tilt <target_value>
            std::cout << "tilting..." << std::endl;
            std::cin >> input;
            std::string::size_type strSize{0};
            uint8_t value = std::stoi(input, &strSize);
            if  (strSize > 0)
                c.tilt(value);
            else
                std::cerr << "Could not parse into unsigned int:" << input << std::endl;

        }
        else if (input  == "w")
        {
            std::cout << "Forward" << std::endl;
            c.moveCar(CarMovement_t::FORWARD);
        }
        else if (input == "a")
        {
            std::cout << "Left" << std::endl;
            c.moveCar(CarMovement_t::LEFT);
        }
        else if (input == "d")
        {
            std::cout << "Right" << std::endl;
            c.moveCar(CarMovement_t::RIGHT);
        }
        else if (input == "s")
        {
            std::cout << "Reverse" << std::endl;
            c.moveCar(CarMovement_t::REVERSE);
        }
        else if (input == "r")
        {
            std::cout << "Stop" << std::endl;
            c.moveCar(CarMovement_t::STOP);
        }
        else
        {
            std::cerr << "Invalid input:" << input << std::endl;
            std::cout << "Stopping..." << std::endl;
            c.moveCar(CarMovement_t::STOP);
            return;
        }

        // Parse stream from stdin again
    }
}