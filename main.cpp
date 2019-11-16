#include <iostream>
#include <string>
#include <future>

#include "car.hpp"


void runCliInput()
{
    using namespace Car;
    using namespace std::chrono_literals;

    std::string input;

    wiringPiSetup();

    Car::Car c;
    std::cout << "Starting CLI loop..." << std::endl;
    int i{0};



    while (1)
    {
        std::cin >> input;

        // Big ol if/else so i can test each motor
        if (input == "end")
        {
            std::cout << "Returning..." << std::endl;
            c.moveCar(CarMovement_t::STOP);
            return;
        }

        if (input == "pan")
        {
            // pan <target_value>
            std::cout << "panning..." << std::endl;
            std::cin >> input;
            std::string::size_type strSize{0};
            unsigned int value = std::stoi(input, &strSize);
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
            unsigned int value = std::stoi(input, &strSize);
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
        else if (input == "b")
        {
            std::cout << "Beep for 1 sec" << std::endl;
            c.beep(1s);
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

int main(int argc, char** argv)
{
    runCliInput();
    return 0;
}
