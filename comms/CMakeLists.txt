# Build an object file from this
add_library(obj_comms OBJECT irRemoteParser.cpp)

target_include_directories(obj_comms PUBLIC ${car_include_dirs}
                                            ${wiringpi_SOURCE_DIR}/wiringPi
                                            ${ws2811_SOURCE_DIR})