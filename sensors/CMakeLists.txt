# Build an object file from this
add_library(obj_sensors OBJECT init.cpp
                               sensors.cpp)

target_include_directories(obj_sensors PUBLIC ${car_include_dirs}
                                              ${wiringpi_SOURCE_DIR}/wiringPi
                                              ${ws2811_SOURCE_DIR})