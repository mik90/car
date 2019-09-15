### Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

set(HEADER_LIST "${PROJECT_SOURCE_DIR}/include/*.hpp")

add_subdirectory(car)
add_subdirectory(comms)
add_subdirectory(control)
add_subdirectory(sensors)
add_subdirectory(util)

include(CMakePrintHelpers)

# Add the object files from each folder
add_executable(car_controller main.cpp
    $<TARGET_OBJECTS:obj_car>
    $<TARGET_OBJECTS:obj_comms>
    $<TARGET_OBJECTS:obj_control>
    $<TARGET_OBJECTS:obj_sensors>
    $<TARGET_OBJECTS:obj_util>)

# Allow control through the command line
add_executable(car_cli main.cpp
    $<TARGET_OBJECTS:obj_car>
    $<TARGET_OBJECTS:obj_comms>
    $<TARGET_OBJECTS:obj_control>
    $<TARGET_OBJECTS:obj_sensors>
    $<TARGET_OBJECTS:obj_util>)


target_compile_features(car_controller PRIVATE cxx_std_17)

target_include_directories(car_controller PRIVATE ${PROJECT_SOURCE_DIR}/include
                                                  ${wiringpi_SOURCE_DIR}/wiringPi 
                                                  ${ws2811_SOURCE_DIR})

# TODO, make wiringPi autogen a symlink
find_library(wiringPi_LIB NAMES libwiringPi.so.2.46 wiringPi PATHS ${wiringpi_SOURCE_DIR}/wiringPi NO_DEFAULT_PATH)
if(NOT wiringPi_LIB)
    message(FATAL_ERROR "Could not find libwiringPi in ${wiringpi_SOURCE_DIR}/wiringPi")
endif()
find_library(ws2811_LIB NAMES ws2811 PATHS ${ws2811_SOURCE_DIR}  NO_DEFAULT_PATH)
if(NOT ws2811_LIB)
    message(FATAL_ERROR "Could not find libws2811 in ${ws2811_SOURCE_DIR}")
endif()

target_link_libraries(car_controller PRIVATE ${wiringPi_LIB} ${ws2811_LIB})

source_group(TREE "${PROJECT_SOURCE_DIR}/include/" PREFIX "Header files" FILES ${HEADER_LIST})