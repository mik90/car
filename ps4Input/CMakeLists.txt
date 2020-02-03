project(ps4Input VERSION 0.0.1
        DESCRIPTION "Handles PS4 Input"
        LANGUAGES C CXX)

# ------------------------

add_executable(evDevTester tester.cpp)

set_target_properties(evDevTester PROPERTIES CXX_STANDARD 17
                                               COMPILE_OPTIONS -Wall)
target_include_directories(evDevTester PRIVATE /usr/include/libevdev-1.0)
target_link_libraries(evDevTester PRIVATE evdev)

# ------------------------

add_executable(ps4Handler eventHandler.cpp)
set_target_properties(ps4Handler PROPERTIES CXX_STANDARD 17
                                               COMPILE_OPTIONS -Wall)
target_include_directories(ps4Handler PRIVATE /usr/include/libevdev-1.0)
target_link_libraries(ps4Handler PRIVATE evdev)
