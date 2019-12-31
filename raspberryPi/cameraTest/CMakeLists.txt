cmake_minimum_required(VERSION 3.13)
find_package(OpenCV)

add_executable(cameraTest cameraTest.cpp)
target_include_directories(cameraTest PUBLIC ${OpenCV_INCLUDE_DIRS})
set_target_properties(cameraTest PROPERTIES CXX_STANDARD 14)
target_link_libraries(cameraTest PUBLIC ${OpenCV_LIBS})