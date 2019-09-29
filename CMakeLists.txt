cmake_minimum_required (VERSION 3.13)
project(Car
        VERSION 0.0.1
        DESCRIPTION "Raspberry-pi powered toy car"
        LANGUAGES C CXX)



set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Making the main project
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set (CXX_EXTENSIONS OFF)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

endif()


include(FetchContent)

###################################
FetchContent_Declare(
  wiringPi
  GIT_REPOSITORY https://github.com/WiringPi/WiringPi.git
  # There are no available tags for WiringPi
  GIT_TAG        origin/master
)
FetchContent_GetProperties(wiringPi)
if(NOT wiringpi_POPULATED)
  FetchContent_Populate(wiringPi)
  # Build the library
  execute_process(COMMAND make
                  WORKING_DIRECTORY ${wiringpi_SOURCE_DIR}/wiringPi)
endif()

###################################
FetchContent_Declare(
  ws2811 
  GIT_REPOSITORY https://github.com/jgarff/rpi_ws281x.git
  # There are no available tags for ws281x 
  GIT_TAG        origin/master
  # Note: there is a typo in the Scons buildscript
  # where 0755 needs to be "0755"
)
FetchContent_GetProperties(ws2811)
if(NOT ws2811_POPULATED)
  FetchContent_Populate(ws2811)
  # Manually fix typo in buildfile
  file (READ ${ws2811_SOURCE_DIR}/SConscript filedata)
  string(REGEX REPLACE ", 0755" ", \"0755\"" filedata "${filedata}")
  file (WRITE ${ws2811_SOURCE_DIR}/SConscript ${filedata})
  # Build the library
  execute_process(COMMAND scons
                  WORKING_DIRECTORY ${ws2811_SOURCE_DIR})

endif()
###################################


# Car implementation is here
add_subdirectory(src)
