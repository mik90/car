project(car)
cmake_minimum_required (VERSION 3.1)

#include(${CMAKE_CURRENT_SOURCE_DIR}/toolchain-rpi.cmake)

#set(CMAKE_CROSSCOMPILING TRUE)
# TODO Find newer cross-compiler
# TODO use system host if on Pi, have some type of debug mode for x86
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_VERBOSE_MAKEFILE ON)

#include(GoogleTest)

# Grab the cross-compiled wiringPi and ws2811 libraries
# Do not grab the system versions
find_library(arm_wiringPi_LIB wiringPi HINTS deps/wiringPi/arm_lib NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
if(NOT arm_wiringPi_LIB)
#    message(FATAL_ERROR "Could not find arm wiringPi library")
endif()
find_library(arm_ws2811_LIB ws2811 HINTS deps/ws2811/arm_lib NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
if(NOT arm_ws2811_LIB)
#    message(FATAL_ERROR "Could not find arm ws2811 library")
endif()
# Wiring Pi includes
set(wiringPi_INCLUDE_DIR deps/wiringPi/include)
# ws2811 includes
set(ws2811_INCLUDE_DIR deps/ws2811/include)


### Doxygen Logic ####
find_package(Doxygen)
# Check if we can make doxygen output
if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${PROJECT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${PROJECT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Building doxygen...")

    add_custom_target(doxygen ALL
                      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
                      COMMENT "Generating API documentation with Doxygen"
                      VERBATIM)
endif (DOXYGEN_FOUND)

add_compile_options(-pedantic -Werror)

# Car directory and sources
set(car_DIR "src/car")
file(GLOB car_SRC "${car_DIR}/*.cpp")

# Comms directory and sources
set(comms_DIR "src/comms")
file(GLOB comms_SRC "${comms_DIR}/*.cpp")

# Control directory and sources
set(control_DIR "src/control")
file(GLOB control_SRC "${control_DIR}/*.cpp")

# Gtest directory and sources
#set(gtest_DIR "src/gtest")
#file(GLOB gtest_SRC "${gtest_DIR}/*.cpp")

# Sensors directory and sources
set(sensors_DIR "src/sensors")
file(GLOB sensors_SRC "${sensors_DIR}/*.cpp")

# Util directory and sources
set(util_DIR "src/util")
file(GLOB util_SRC "${util_DIR}/*.cpp")

# Main source
set(main_SRC "src/main.cpp")

set(car_SRC
  ${car_SRC} ${comms_SRC} ${control_SRC} ${sensors_SRC} ${util_SRC})
set(car_INCLUDE_DIRS
  ${car_DIR} ${comms_DIR} ${control_DIR} ${sensors_DIR} ${util_DIR})

# Roll up sources and includes for driver binary
set(driver_SRC ${main_SRC} ${car_SRC})

# Add internal includes as well as includes of dependencies
include_directories(BEFORE ${car_INCLUDE_DIRS}
                           ${wiringPi_INCLUDE_DIR}
                           ${ws2811_INCLUDE_DIR})

# Create our executables
add_executable(driver       ${driver_SRC})
add_executable(debug_driver ${driver_SRC})

# Disabling gtest for now
#add_executable(gtest_driver ${gtest_SRC} ${car_SRC})
#gtest_add_tests(TARGET       gtest_driver
#                TEST_SUFFIX .noArgs
#                TEST_LIST    noArgsTests)
#set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 10)

# Adjust compile options
target_compile_options(driver       PUBLIC -O2)
target_compile_options(debug_driver PUBLIC -g)


target_link_libraries(driver       ${arm_wiringPi_LIB} ${arm_ws2811_LIB})
target_link_libraries(debug_driver ${arm_wiringPi_LIB} ${arm_ws2811_LIB})

#target_link_libraries(gtest_driver gtest gtest_main)