#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <ArduinoJson.h>


Adafruit_MotorShield shield;

auto frontRightMotor = shield.getMotor(1); // M1
auto frontLeftMotor  = shield.getMotor(2); // M2
auto rearRightMotor  = shield.getMotor(3); // M3
auto rearLeftMotor   = shield.getMotor(4); // M4
uint32_t readUltrasonicSensor();
constexpr int baudRate = 9600;
TwoWire i2c;
constexpr int myI2cAddress = 2;

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
    i2c.begin(myI2cAddress);
    i2c.onReceive(receiveEvent);
    shield.begin(1600, &i2c);
    Serial.begin(baudRate);

    frontLeftMotor->run(RELEASE); 
    rearLeftMotor->run(RELEASE); 
    frontRightMotor->run(RELEASE); 
    rearRightMotor->run(RELEASE); 

    setMotorSpeeds(0);
    while (!Serial) {
        // Wait
    }
    Serial.println("motorController initialized.");
}

void loop() {
    delay(100);
}

void receiveEvent(int nBytes) {
    // Example data: 150,0,0\0

    while (i2c.available()) {
        // 0 - 255
        String speed = i2c.readStringUntil(',');
        setMotorSpeeds(speed.toInt());
       
        String left_side_dir = i2c.readStringUntil(',');
        setLeftSideDir(left_side_dir.toInt());
        
        String right_side_dir = i2c.readStringUntil('\0');
        setRightSideDir(right_side_dir.toInt());
        Serial.println("Received I2C data: " + speed + "," + left_side_dir
                        + "," + right_side_dir);
    }
}
#if 0
// Example data:
// char json[] = "{\"wheel_speed\":100,\"left_side_dir\":\"Forward\",\"right_side_dir\":\"Forward\"}";
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
void loop() {
    // Example data:
    // char json[] = "{\"wheel_speed\":100,\"left_side_dir\":\"Forward\",\"right_side_dir\":\"Forward\"}";

    StaticJsonDocument<200> doc;
    auto json = Serial.readStringUntil('\0');
    DeserializationError error = deserializeJson(doc, json);

    if (error) {
        /*Serial.print("Could not deserialize JSON. ");
        Serial.println(error.c_str());
        Serial.flush();*/
        // Just wait for the next iteration
        return;
    }

    String speed = doc["wheel_speed"];
    setMotorSpeeds(speed.toInt());
    String left_side_dir = doc["left_side_dir"];
    setLeftSideDir(stringToCommand(left_side_dir));

    String right_side_dir = doc["right_side_dir"];
    setRightSideDir(stringToCommand(right_side_dir));
}
#endif