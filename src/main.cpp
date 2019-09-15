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
        if (input.find("w") != string::npos)
        {
            cout << "Forward" << endl;
            c.moveCar(Car::CarDirection_t::FORWARD);
        }
        else if (input.find("a") != string::npos)
        {
            cout << "Left" << endl;
            c.moveCar(Car::CarDirection_t::LEFT);
        }
        else if (input.find("d") != string::npos)
        {
            cout << "Right" << endl;
            c.moveCar(Car::CarDirection_t::RIGHT);
        }
        else if (input.find("s") != string::npos)
        {
            cout << "Reverse" << endl;
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
    runCliInput();
    return 0;
}