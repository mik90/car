# Car controller that runs on the raspberry pi 
project(CarController VERSION 0.0.1
        DESCRIPTION "Raspberry-pi based interface to the motor controller and Pi camera"
        LANGUAGES C CXX)

add_executable(carController main.cpp
                             commandArduino.cpp
                             car.cpp)

set_target_properties(carController PROPERTIES CXX_STANDARD 14
                                               COMPILE_OPTIONS "-Wall -Werror"
                                               POSITION_INDEPENDENT_CODE ON)
                             
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -O0 -DDEBUG=YES" CACHE STRING "Debug C++ flags")
set(CMAKE_CXX_FLAGS_RELEASE "-O2" CACHE STRING "Release C++ flags")