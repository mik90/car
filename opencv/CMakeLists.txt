cmake_minimum_required (VERSION 3.13)
project(ImageHandler VERSION 0.0.1
        DESCRIPTION "Image processor"
        LANGUAGES C CXX)

# Get external dependencies
find_package(OpenCV REQUIRED)

add_executable(imageHandler imageHandler.cpp)
target_include_directories(imageHandler PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(imageHandler PUBLIC ${OpenCV_LIBS})

add_executable(faceRecognizer faceRecognizer.cpp)
target_include_directories(faceRecognizer PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(faceRecognizer PUBLIC ${OpenCV_LIBS})