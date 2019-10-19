project(car)
cmake_minimum_required (VERSION 3.13)

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

# TODO, make wiringPi autogen a symlink to libwiringPi.so on build
find_library(wiringPi_LIB NAMES libwiringPi.so.2.46 wiringPi PATHS ${wiringpi_SOURCE_DIR}/wiringPi NO_DEFAULT_PATH)
if(NOT wiringPi_LIB)
    message(FATAL_ERROR "Could not find libwiringPi in ${wiringpi_SOURCE_DIR}/wiringPi, may need to create a symlink to libwiringPi.so")
endif()
find_library(ws2811_LIB NAMES ws2811 PATHS ${ws2811_SOURCE_DIR}  NO_DEFAULT_PATH)
if(NOT ws2811_LIB)
    message(FATAL_ERROR "Could not find libws2811 in ${ws2811_SOURCE_DIR}")
endif()
