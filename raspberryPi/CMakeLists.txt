# Car controller that runs on the raspberry pi 
project(CarController VERSION 0.0.1
        DESCRIPTION "Raspberry-pi based interface to the motor controller and Pi camera"
        LANGUAGES C CXX)

find_package(OpenCV REQUIRED)

add_executable(carController main.cpp
                             arduinoInterface.cpp
                             car.cpp
                             faceDetector.cpp)

set_target_properties(carController PROPERTIES CXX_STANDARD 14
                                               COMPILE_OPTIONS -Wall
                                               POSITION_INDEPENDENT_CODE ON)
target_include_directories(carController PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(carController PUBLIC ${OpenCV_LIBS})