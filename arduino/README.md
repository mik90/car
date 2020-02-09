### MotorController ###
- Arduino UNO Rev3 (AVR)
- arduino:avr:uno

### CarWifi ###
- Arduino Nano 33 IoT (SAMD)
- arduino:samd:arduino_zero_native (Unsure if this is right)

### Using arduino-cli for library management ###

### TODO ###
- Read in messages with JSON

- Weird issues
- /usr/share/arduino/lib/version.txt doesn't have the version so manually override it
- avr-size for Gentoo and Arch don't have the -C option
    - In SETUP_ARDUINO_SIZE_SCRIPT in arduino-cmake/cmake/Platform/Arduino.cmake
      it has "set(AVRSIZE_FLAGS -C --mcu=\${MCU})" which won't work for some distros
    - The firmware hex will still be made, but it will throw an annoying error at the
      end of linking and not tell you the firmware size