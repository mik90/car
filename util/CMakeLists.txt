# Build an object file from this
add_library(obj_util OBJECT led.cpp
                            common_rpi.cpp)

target_include_directories(obj_util PUBLIC ${car_include_dirs}
                                           ${wiringpi_SOURCE_DIR}/wiringPi
                                           ${ws2811_SOURCE_DIR})