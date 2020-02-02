project(ps4Input VERSION 0.0.1
        DESCRIPTION "Handles PS4 Input"
        LANGUAGES C CXX)

# ------------------------

add_executable(ps4Tester eventHandler.cpp)

set_target_properties(ps4Tester PROPERTIES CXX_STANDARD 14
                                               COMPILE_OPTIONS -Wall
                                               POSITION_INDEPENDENT_CODE ON)
target_include_directories(ps4Tester PRIVATE /usr/include/libevdev-1.0)
target_link_libraries(ps4Tester PRIVATE evdev)