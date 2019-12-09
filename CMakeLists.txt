cmake_minimum_required (VERSION 3.13)
SET(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/RaspberryPiToolchain.cmake)
project(Car VERSION 0.0.1
        DESCRIPTION "Raspberry-pi and Arduino powered toy car"
        LANGUAGES C CXX)

# Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file)."
                        " Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()
# This is the project-level CMakeLists.txt that pulls in dependencies and sets meta-information about the project 
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Get external dependencies
find_package(Threads REQUIRED)

set(extern_DIR GLOBAL ${CMAKE_SOURCE_DIR}/extern)

add_subdirectory(raspberryPi)