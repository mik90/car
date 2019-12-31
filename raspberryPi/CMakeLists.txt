# Car controller that runs on the raspberry pi 
project(CarController VERSION 0.0.1
        DESCRIPTION "Raspberry-pi based interface to the motor controller and Pi camera"
        LANGUAGES C CXX)

find_package(OpenCV REQUIRED)

# ------------------------

add_executable(cliCarController cliMain.cpp
                                arduinoInterface.cpp
                                car.cpp
                                faceDetector.cpp)

set_target_properties(cliCarController PROPERTIES CXX_STANDARD 14
                                               COMPILE_OPTIONS -Wall
                                               POSITION_INDEPENDENT_CODE ON)
target_include_directories(cliCarController PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(cliCarController PUBLIC ${OpenCV_LIBS})

# ------------------------

add_executable(faceCarController faceMain.cpp
                                 arduinoInterface.cpp
                                 car.cpp
                                 faceDetector.cpp)

set_target_properties(faceCarController PROPERTIES CXX_STANDARD 14
                                               COMPILE_OPTIONS -Wall
                                               POSITION_INDEPENDENT_CODE ON)
target_include_directories(faceCarController PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(faceCarController PUBLIC ${OpenCV_LIBS})