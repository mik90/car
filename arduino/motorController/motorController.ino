#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

Adafruit_MotorShield shield;

auto frontRightMotor = shield.getMotor(1); // M1
auto frontLeftMotor  = shield.getMotor(2); // M2
auto rearRightMotor  = shield.getMotor(3); // M3
auto rearLeftMotor   = shield.getMotor(4); // M4
constexpr int baudRate = 9600;
SoftwareSerial controlSerial(2, 3); // RX, TX Digital Pins

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
    Serial.begin(baudRate);

    shield.begin();
    frontLeftMotor->run(RELEASE); 
    rearLeftMotor->run(RELEASE); 
    frontRightMotor->run(RELEASE); 
    rearRightMotor->run(RELEASE); 

    setMotorSpeeds(0);
    
	controlSerial.begin(9600);
    Serial.println("motorController initialized.");
}
void loop() {
      // Example data: 150,0,0\0

    if (controlSerial.available() > 0) {
        Serial.println("Received " + String(controlSerial.available()) + " bytes");
        // 0 - 255
        uint8_t speed = controlSerial.read();
        controlSerial.read();
        uint8_t left_side_dir = controlSerial.read();
        controlSerial.read();
        uint8_t right_side_dir = controlSerial.read();
        controlSerial.read();
        
        Serial.println("Received control data: " + String(speed) + "," + String(left_side_dir)
                        + "," + String(right_side_dir));
        setMotorSpeeds(speed);
        setLeftSideDir(left_side_dir);
        setRightSideDir(right_side_dir);
        Serial.println("Sent commands to motors");
    }
    delay(100);
  
}
