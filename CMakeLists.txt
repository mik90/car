project(car)
cmake_minimum_required (VERSION 3.1)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
set(CMAKE_GENERATOR "Ninja")

include(GoogleTest)

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

# Util directory and sources
set(util_DIR "src/util")
file(GLOB util_SRC "${util_DIR}/*.cpp")

# Gtest directory and sources
set(gtest_DIR "src/gtest")
file(GLOB gtest_SRC "${gtest_DIR}/*.cpp")

# Dependencies
set(deps_DIR "deps")

# Main source
file(GLOB main_SRC "src/main.cpp")

# Roll up sources and includes for driver binary
set(driver_SRC ${main_SRC} ${util_SRC})
set(driver_include_dirs ${util_DIR} ${deps_DIR})

include_directories(BEFORE ${driver_include_dirs})

# Create our executables
add_executable(driver ${driver_SRC})
add_executable(debug_driver ${driver_SRC})
add_executable(gtest_driver ${gtest_SRC} ${util_SRC})
gtest_add_tests(TARGET       gtest_driver
                TEST_SUFFIX .noArgs
                TEST_LIST    noArgsTests)
set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 10)


# Adjust compile options
target_compile_options(driver PUBLIC -O2)
target_compile_options(debug_driver PUBLIC -g)

target_link_libraries(gtest_driver gtest gtest_main)

# Adjust permissions
set(permissions_default
    OWNER_WRITE OWNER_READ OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)
# Install with "ninja install"
install(TARGETS driver debug_driver gtest_driver
        CONFIGURATIONS Release Debug
        RUNTIME DESTINATION bin
        PERMISSIONS ${permissions_default})