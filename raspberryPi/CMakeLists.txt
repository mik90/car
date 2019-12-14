# Car controller that runs on the raspberry pi 


add_executable(carController main.cpp
                              car.cpp)

find_library(wiringPiLib NAMES libwiringPi.so
                         PATHS ${extern_DIR}/WiringPi/lib
                         NO_DEFAULT_PATH)

set_target_properties(carController PROPERTIES CXX_STANDARD 14
                                               POSITION_INDEPENDENT_CODE ON
                                               RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
                             
target_compile_options(carController PUBLIC -Wall )

target_include_directories(carController PUBLIC ${CMAKE_CURRENT_LIST_DIR}
                                                 ${extern_DIR}/WiringPi/include)

target_link_libraries(carController PUBLIC Threads::Threads
                                           ${wiringPiLib})