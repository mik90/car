#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <ArduinoJson.h>


Adafruit_MotorShield shield;

auto frontRightMotor = shield.getMotor(1); // M1
auto frontLeftMotor  = shield.getMotor(2); // M2
auto rearRightMotor  = shield.getMotor(3); // M3
auto rearLeftMotor   = shield.getMotor(4); // M4
uint32_t readUltrasonicSensor();

StaticJsonDocument<200> doc;

int stringToCommand(const String& str) {
    if (str.equalsIgnoreCase("Forward")) {
        return FORWARD;
    }
    else if (str.equalsIgnoreCase("Reverse")) {
        return BACKWARD;
    }
    else {
        return RELEASE;
    }
}

void setMotorSpeeds(int speed) {
    frontLeftMotor->setSpeed(speed);
    rearLeftMotor->setSpeed(speed);
    frontRightMotor->setSpeed(speed);
    rearRightMotor->setSpeed(speed);
}
void setLeftSideDir(int dir) {
    frontLeftMotor->run(dir); 
    rearLeftMotor->run(dir); 
}
void setRightSideDir(int dir) {
    frontRightMotor->run(dir); 
    rearRightMotor->run(dir); 
}

void setup() {
    shield.begin();
    Serial.begin(9600);

    frontLeftMotor->run(RELEASE); 
    rearLeftMotor->run(RELEASE); 
    frontRightMotor->run(RELEASE); 
    rearRightMotor->run(RELEASE); 

    setMotorSpeeds(0);
    while (!Serial) continue;

}

void loop() {
    auto json = Serial.readString();
    DeserializationError error = deserializeJson(doc, json);

    setMotorSpeeds(doc["wheelSpeed"]);
    setLeftSideDir(doc["leftSideDir"]);
    setRightSideDir(doc["rightSideDir"]);
}
