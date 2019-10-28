cmake_minimum_required (VERSION 3.13)
# Make sure that Cmake's default mode includes C++17
# Otherwise it complains about some feature availabilities
set(CMAKE_CXX_FLAGS -std=c++17)
project(Car
        VERSION 0.0.1
        DESCRIPTION "Raspberry-pi powered toy car"
        LANGUAGES C CXX)

# Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file)."
                        " Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# This is the project-level CMakeLists.txt that pulls in dependencies and sets meta-information about the project 
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

# Get external dependencies
include(FetchCarDeps)
include(CMakePrintHelpers)
find_package(Threads REQUIRED)

set(car_include_dirs ${CMAKE_SOURCE_DIR}/car
                     ${CMAKE_SOURCE_DIR}/comms
                     ${CMAKE_SOURCE_DIR}/control
                     ${CMAKE_SOURCE_DIR}/sensors
                     ${CMAKE_SOURCE_DIR}/util)

add_subdirectory(car)
add_subdirectory(comms)
add_subdirectory(control)
add_subdirectory(sensors)
add_subdirectory(util)

# Add the object files from each folder
add_executable(car_controller main.cpp
               $<TARGET_OBJECTS:obj_car>
               $<TARGET_OBJECTS:obj_comms>
               $<TARGET_OBJECTS:obj_control>
               $<TARGET_OBJECTS:obj_sensors>
               $<TARGET_OBJECTS:obj_util>)

target_compile_features(car_controller PUBLIC cxx_std_17)

set_target_properties(car_controller PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(car_controller PRIVATE ${car_include_dirs}
                                                  ${wiringpi_SOURCE_DIR}/wiringPi 
                                                  ${ws2811_SOURCE_DIR})

target_link_libraries(car_controller PRIVATE Threads::Threads
                                             ${wiringPi_LIB}
                                             ${ws2811_LIB})

set_target_properties(car_controller PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

source_group(TREE "${PROJECT_SOURCE_DIR}/include/" PREFIX "Header files" FILES ${HEADER_LIST})