#include <iostream>
#include <string>
#include <thread>

#include "car.hpp"

int runCliLoop()
{
    using namespace Car;
    using namespace std::chrono_literals;

    Car::Car c;
    std::cout << "Starting CLI loop..." << std::endl;

    std::string input;
    while (true)
    {
        std::cin >> input;

        if (input == "end")
        {
            std::cout << "Returning..." << std::endl;
            c.moveCar(CarMovement_t::STOP);
            return 0;
        }
        else if (input == "speed")
        {
            std::cout << "changing speed..." << std::endl;
            std::cin >> input;
            std::string::size_type strSize{0};
            uint8_t value = std::stoi(input, &strSize);
            if (strSize > 0)
                c.setSpeed(value);
            else
                std::cerr << "Could not parse into unsigned int:" << input << std::endl;
        }
        else if (input == "pan")
        {
            // pan <target_value>
            std::cout << "panning..." << std::endl;
            std::cin >> input;
            std::string::size_type strSize{0};
            uint8_t value = std::stoi(input, &strSize);
            if (strSize > 0)
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
        else if (input == "stop")
        {
            std::cout << "Stop" << std::endl;
            c.moveCar(CarMovement_t::STOP);
        }
        else
        {
            std::cerr << "Invalid input:" << input << std::endl;
            std::cout << "Stopping..." << std::endl;
            c.moveCar(CarMovement_t::STOP);
            return 1;
        }

        // Parse stream from stdin again
    }
    
    return 1;
}

int main(int argc, char** argv)
{
    return runCliLoop();
}
