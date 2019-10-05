### Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

include(FetchContent)

set (car_src_files
    car.cpp
)

# We aren't building a full blown library with this folder,
# only object files
add_library(obj_car OBJECT ${car_src_files})

target_include_directories(obj_car PUBLIC ${PROJECT_SOURCE_DIR}/include
                                          ${wiringpi_SOURCE_DIR}/wiringPi
                                          ${ws2811_SOURCE_DIR})