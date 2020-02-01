find_package(OpenCV QUIET)

add_executable(faceCarController faceMain.cpp
                                 faceDetector.cpp
                                 ../arduinoInterface.cpp
                                 ../car.cpp)

set_target_properties(faceCarController PROPERTIES CXX_STANDARD 14
                                                   COMPILE_OPTIONS -Wall
                                                   POSITION_INDEPENDENT_CODE ON)
target_compile_definitions(faceCarController PRIVATE -DUSE_FACE_DETECTION)
target_include_directories(faceCarController PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(faceCarController PUBLIC ${OpenCV_LIBS})