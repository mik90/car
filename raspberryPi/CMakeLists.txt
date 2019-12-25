# Car controller that runs on the raspberry pi 


add_executable(carController main.cpp
                              commandArduino.cpp
                              car.cpp)

set_target_properties(carController PROPERTIES CXX_STANDARD 14
                                               COMPILE_OPTIONS -Wall
                                               POSITION_INDEPENDENT_CODE ON
                                               RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
                             
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -O0 -DDEBUG=YES" CACHE STRING "Debug C++ flags")
set(CMAKE_CXX_FLAGS_RELEASE "-O2" CACHE STRING "Release C++ flags")