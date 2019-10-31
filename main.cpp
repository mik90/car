#include <iostream>
#include <string>
#include <future>

#include "car.hpp"


void runCliInput()
{
    using namespace Car;
    using namespace std::chrono_literals;

    std::string input;
    Car::Car c;
    std::cout << "Starting CLI loop..." << std::endl;
    int i{0};
    while (1)
    {
        std::cin >> input;

        // Big ol if/else so i can test each motor
        if (input.find("end") != std::string::npos)
        {
            std::cout << "Returning..." << std::endl;
            c.moveCar(CarMovement_t::STOP);
            return;
        }
        else if (input.find("panl") != std::string::npos)
        {
            std::cout << "panning left..." << std::endl;
            c.panLeft();
        }
        else if (input.find("panr") != std::string::npos)
        {
            std::cout << "panning right..." << std::endl;
            c.panRight();
        }
        else if (input.find("tiltu") != std::string::npos)
        {
            std::cout << "tilting up..." << std::endl;
            c.tiltUp();
        }
        else if (input.find("tiltd") != std::string::npos)
        {
            std::cout << "tilting down..." << std::endl;
            c.tiltDown();
        }
        else if (input.find("w") != std::string::npos)
        {
            std::cout << "Forward" << std::endl;
            c.moveCar(CarMovement_t::FORWARD);
        }
        else if (input.find("a") != std::string::npos)
        {
            std::cout << "Left" << std::endl;
            c.moveCar(CarMovement_t::LEFT);
        }
        else if (input.find("d") != std::string::npos)
        {
            std::cout << "Right" << std::endl;
            c.moveCar(CarMovement_t::RIGHT);
        }
        else if (input.find("s") != std::string::npos)
        {
            std::cout << "Reverse" << std::endl;
            c.moveCar(CarMovement_t::REVERSE);
        }
        else if (input.find("b") != std::string::npos)
        {
            // Result: beeps
            std::cout << "Beep for 1 sec" << std::endl;
            c.beep(1s);
        }
        else if (input.find("r") != std::string::npos)
        {
            // Result: stops (at least FR does)
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
    }
}

int main(int argc, char** argv)
{
    runCliInput();
    return 0;
}
