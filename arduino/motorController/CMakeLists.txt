project(MotorController VERSION 0.0.1
        DESCRIPTION "Arduino-based DC motor controller"
        LANGUAGES C CXX)

generate_arduino_firmware(MotorController
    BOARD uno
    SKETCH motorController.ino)
