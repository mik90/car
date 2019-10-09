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
        std::cout << "Loop #" << i++ << std::endl;
        std::cin >> input;
        if (input.find("w") != std::string::npos)
        {
            std::cout << "Forward" << std::endl;
            c.moveCar(CarDirection_t::FORWARD);
        }
        else if (input.find("a") != std::string::npos)
        {
            std::cout << "Left" << std::endl;
            c.moveCar(CarDirection_t::LEFT);
        }
        else if (input.find("d") != std::string::npos)
        {
            std::cout << "Right" << std::endl;
            c.moveCar(CarDirection_t::RIGHT);
        }
        else if (input.find("s") != std::string::npos)
        {
            std::cout << "Reverse" << std::endl;
            c.moveCar(CarDirection_t::REVERSE);
        }
        else if (input.find("b") != std::string::npos)
        {
            std::cout << "Beep for 1 sec" << std::endl;
            c.beepSeconds(1s);
        }
        else
        {
            std::cerr << "Invalid input:" << input << std::endl;
            return;
        }
    }
}

int main(int argc, char** argv)
{
    runCliInput();
    return 0;
}