# Build an object file from this
add_library(obj_comms OBJECT pi_client.cpp
                             server.cpp)

target_include_directories(obj_comms PUBLIC ${car_include_dirs}
                                            ${wiringpi_SOURCE_DIR}/wiringPi
                                            ${ws2811_SOURCE_DIR})