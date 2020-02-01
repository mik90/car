# Car controller that runs on the raspberry pi 
project(CarController VERSION 0.0.1
        DESCRIPTION "Raspberry-pi based interface to the motor controller and Pi camera"
        LANGUAGES C CXX)

# ------------------------

add_executable(cliCarController cliMain.cpp
                                arduinoInterface.cpp
                                car.cpp)

set_target_properties(cliCarController PROPERTIES CXX_STANDARD 14
                                               COMPILE_OPTIONS -Wall
                                               POSITION_INDEPENDENT_CODE ON)

# ------------------------

# Disabled for now, haven't added OpenCV as a CMake project to the Rpi toolchain
# add_subdirectory(faceDetector)