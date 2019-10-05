### Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

set (control_src_files
    init.cpp
    effectors.cpp
)

# We aren't building a full blown library with this folder,
# only object files
add_library(obj_control OBJECT ${control_src_files})

target_include_directories(obj_control PUBLIC ${PROJECT_SOURCE_DIR}/include
                                              ${wiringpi_SOURCE_DIR}/wiringPi
                                              ${ws2811_SOURCE_DIR})