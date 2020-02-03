#include <fcntl.h>

#include <cerrno>
#include <cstring>
#include <csignal>

#include <memory>
#include <iostream>
#include <string>
#include <optional>

#include <libevdev/libevdev.h>

// Reference: https://github.com/chrippa/ds4drv/blob/master/ds4drv/uinput.py
// Has evdev mapping

// Reference: https://gitlab.freedesktop.org/libevdev/libevdev/blob/master/tools/libevdev-events.c
// Evdev example

// /dev/input/by-id/usb-Sony_Computer_Entertainment_Wireless_Controller-<some_value>
// Only event- ones are readable by livevdev as they're events
// event-if00 is the motion sensor
// event-joystick is the traditional controller interface
// event-mouse is the touchpad

static struct libevdev* dev = nullptr;

void interruptHandler(int signal)
{
    libevdev_free(dev);
    std::cout << "Caught signal:" << signal << std::endl;
}

void initLibEvDev()
{
    constexpr char eventFilePath[] = "/dev/input/by-id/usb-Sony_Computer_Entertainment_Wireless_Controller-event-joystick";
    int fd = open(eventFilePath, O_RDONLY);
    if (fd < 0)
    {
        std::cerr << "Could not open \"" << eventFilePath << "\"\n";
        std::cerr << std::strerror(errno) << "\n";
        libevdev_free(dev);
        std::exit(1);
    }

    int evdevStatus = libevdev_new_from_fd(fd, &dev);
    if (evdevStatus < 0)
    {
        std::cerr << "Failed to init evdev:" << std::strerror(errno) << "\n";
        libevdev_free(dev);
        std::exit(1);
    }

}

std::optional<int> findAbsEventValue(unsigned int eventCode)
{
    int value = 0;
    if (libevdev_fetch_event_value(dev, EV_ABS, eventCode, &value) == 0)
        return {};
    else
        return value;
}

struct controllerStatus
{
    // 127 means the stick is straight in the air
    controllerStatus() : leftStick_Xaxis (127), rightStick_Xaxis(127), 
                         leftStick_Yaxis (127), rightStick_Yaxis(127),
                         leftTrigger(0),        rightTrigger(0) { }
    
    int leftStick_Xaxis;
    int rightStick_Xaxis;
    
    int leftStick_Yaxis;
    int rightStick_Yaxis;
   
    int leftTrigger;
    int rightTrigger;
};

std::ostream& operator<<(std::ostream& os, const controllerStatus& cStat)
{
    os << "Left Stick::  (X, Y):" << cStat.leftStick_Xaxis  << ", "  << cStat.leftStick_Yaxis << "\n"
       << "Right Stick:: (X, Y):" << cStat.rightStick_Xaxis << ", " << cStat.rightStick_Yaxis << "\n"
       << "Triggers::    (L, R):" << cStat.leftTrigger      << ", " << cStat.rightTrigger     << "\n";
    return os;
}

int main(int argc, char** argv)
{
    initLibEvDev();
    std::cout << "Initialized libevdev\n";

    std::signal(SIGINT, interruptHandler);

    int evdevStatus = 1;
    do
    {
        struct input_event ev;
        evdevStatus = libevdev_next_event(dev,
                                          LIBEVDEV_READ_FLAG_NORMAL | LIBEVDEV_READ_FLAG_BLOCKING,
                                          &ev);
        if (evdevStatus == LIBEVDEV_READ_STATUS_SUCCESS && ev.type == EV_ABS)
        {
            controllerStatus cStat{};
            std::cout << "Saw EV_ABS event\n";
            // These events are all EV_ABS
            // give or take ~20 due to sensitivity
            // joystick x axis: 0(left) - 127 (middle) - 255 (right)
            // joystick y axis: 255(down) - 127 (middle) - 0 (up)
            auto leftStick_Xaxis = findAbsEventValue(ABS_X);
            if (leftStick_Xaxis.has_value())
                cStat.leftStick_Xaxis = leftStick_Xaxis.value();

            auto leftStick_Yaxis = findAbsEventValue(ABS_Y);
            if (leftStick_Yaxis.has_value())
                cStat.leftStick_Yaxis = leftStick_Yaxis.value();
            
            auto rightStick_Xaxis = findAbsEventValue(ABS_Z);
            if (rightStick_Xaxis.has_value())
                cStat.rightStick_Xaxis = rightStick_Xaxis.value();

            auto rightStick_Yaxis = findAbsEventValue(ABS_RZ);
            if (rightStick_Yaxis.has_value())
                cStat.rightStick_Yaxis = rightStick_Yaxis.value();
            
            // trigger: 0(not pressed) - 255 (fully pressed)
            auto leftTrigger  = findAbsEventValue(ABS_RX);
            if (leftTrigger.has_value())
                cStat.leftTrigger = leftTrigger.value();

            auto rightTrigger = findAbsEventValue(ABS_RY);
            if (rightTrigger.has_value())
                cStat.rightTrigger = rightTrigger.value();

            std::cout << cStat << std::endl;
        }
    } while (evdevStatus == LIBEVDEV_READ_STATUS_SYNC
             || evdevStatus == LIBEVDEV_READ_STATUS_SUCCESS 
             || evdevStatus == -EAGAIN);

    return 1;
}