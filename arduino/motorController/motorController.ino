#include "motorController.hpp"

constexpr int debugSerialBaudRate = 9600;
constexpr int controlSerialBaudRate = 9600;
mik::MotorController controller;

void setup() {
    Serial.begin(debugSerialBaudRate);
    controller.init(controlSerialBaudRate);
    Serial.println("motorController initialized.");
}

void loop() {
    String status = controller.monitorSoftwareSerialPort();
    if (status.length() != 0) {
        Serial.println(status);
    }
    delay(100);
}
