# Build an object file from this
add_library(obj_control OBJECT motorController.cpp
                               pwmWheel.cpp
                               servo.cpp)

target_include_directories(obj_control PUBLIC ${car_include_dirs}
                                              ${wiringpi_SOURCE_DIR}/wiringPi
                                              ${ws2811_SOURCE_DIR})