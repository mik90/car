project(ps4Input VERSION 0.0.1
        DESCRIPTION "Handles PS4 Input"
        LANGUAGES C CXX)

# ------------------------

# This was just a binary from the tools directory of libevdev
#add_executable(evDevTester tester.cpp)

#set_target_properties(evDevTester PROPERTIES CXX_STANDARD 17
#                                               COMPILE_OPTIONS -Wall)
#target_include_directories(evDevTester PRIVATE /usr/include/libevdev-1.0)
#target_link_libraries(evDevTester PRIVATE evdev)

# ------------------------

# This will need to change to run cargo
# example https://michael-f-bryan.github.io/rust-ffi-guide/setting_up.html
add_executable(ps4Handler eventHandler.cpp)
set_target_properties(ps4Handler PROPERTIES CXX_STANDARD 17
                                               COMPILE_OPTIONS -Wall)
target_include_directories(ps4Handler PRIVATE /usr/include/libevdev-1.0)
target_link_libraries(ps4Handler PRIVATE evdev)
