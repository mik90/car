#include <iostream>
#include <string>

#include "car.hpp"


void runCliInput()
{
    using namespace std;
    string input;
    Car::Car c;
    c.initBeep();
    while (1)
    {
        cin >> input;
        if (input == "w")
        {
            c.moveCar(Car::CarDirection_t::FORWARD);
        }
        else if (input == "a")
        {
            c.moveCar(Car::CarDirection_t::LEFT);
        }
        else if (input == "s")
        {
            c.moveCar(Car::CarDirection_t::RIGHT);
        }
        else if (input == "d")
        {

            c.moveCar(Car::CarDirection_t::REVERSE);
        }
        else
        {
            cerr << "Invalid input:" << input << endl;
            return;
        }
    }
}

int main(int argc, char** argv)
{

    return 0;
}