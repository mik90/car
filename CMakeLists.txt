cmake_minimum_required (VERSION 3.13)
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

add_subdirectory(arduino)

enable_testing()
include(GoogleTest)
add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" "extern/googletest")
add_subdirectory(tests)

add_subdirectory(raspberryPi)