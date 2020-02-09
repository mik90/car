register_hardware_platform(${ARDUINO_SDK_PATH}/hardware/arduino/avr)

set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")


include_directories(MotorController 
                ~/Arduino/libraries/ArduinoJson/src
                )
link_directories(MotorController 
        PRIVATE ~/Arduino/libraries
                 /usr/share/arduino/hardware/arduino/avr/libraries/Wire/src
                ~/Arduino/libraries/Adafruit_Motor_Shield_V2_Library
                ~/Arduino/libraries/ArduinoJson
                )
set(ArduinoJson_RECURSE True)
add_subdirectory(motorController)