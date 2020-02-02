#include <libevdev/libevdev.h>
#include <fcntl.h>
#include <memory>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>

// Reference: https://github.com/chrippa/ds4drv/blob/master/ds4drv/uinput.py
// Has evdev mapping

// Reference: https://gitlab.freedesktop.org/libevdev/libevdev/blob/master/tools/libevdev-events.c
// Evdev example

// /dev/input/by-id/usb-Sony_Computer_Entertainment_Wireless_Controller-<some_value>
// Only event- ones are readable by livevdev as they're events
// event-if00 is the motion sensor
// event-joystick is the traditional controller interface
// event-mouse is the touchpad

int main(int argc, char** argv) {
    constexpr char eventFilePath[] = "/dev/input/by-id/usb-Sony_Computer_Entertainment_Wireless_Controller-event-joystick";
    struct libevdev* dev = nullptr;

    int fd = open(eventFilePath, O_RDONLY);
    if (fd < 0) {
        std::cerr << "Could not open \"" << eventFilePath << "\"\n";
        std::cerr << std::strerror(errno) << "\n";
        libevdev_free(dev);
        return 1;
    }

    int evdevStatus = libevdev_new_from_fd(fd, &dev);
    if (evdevStatus < 0) {
        std::cerr << "Failed to init evdev:" << std::strerror(errno) << "\n";
        libevdev_free(dev);
        return 1;
    }

    // Need events:
    // ABS_X              left analog stick x axis
    // ABS_Y              left analog stick y axis

    // ABS_Z              right analog stick x axis
    // ABS_RZ             right analog stick y axis

    // ABS_RX             l2 analog
    // ABS_RY             r2 analog
    
    // give or take ~20 due to sensitivity
    // joystick x axis: 0(left) - 127 (middle) - 255 (right)
    // joystick y axis: 255(down) - 127 (middle) - 0 (up)
    // trigger: 0(not pressed) - 255 (fully pressed)
}