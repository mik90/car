#include <Arduino.h>
#include <Servo.h>
#include <Adafruit_MotorShield.h>

#ifndef ARDUINO
#define ARDUINO
#endif

#include "messages.hpp"

Adafruit_MotorShield shield;
Servo panServo;
Servo tiltServo;
constexpr int UltrasonicEchoPin = 7; // D7
constexpr int UltrasonicTriggerPin = 8; // D8

auto frontRightMotor = shield.getMotor(1); // M1
auto frontLeftMotor  = shield.getMotor(2); // M2
auto rearRightMotor  = shield.getMotor(3); // M3
auto rearLeftMotor   = shield.getMotor(4); // M4
void handleWheelControl(const uint8_t* msg);
void handleServoControl(const uint8_t* msg);
void handleServoRequest(const uint8_t* msg);
uint32_t readUltrasonicSensor();

using namespace motorControllerApi;

void setup()
{
    shield.begin();

    pinMode(UltrasonicEchoPin, INPUT);
    pinMode(UltrasonicTriggerPin, OUTPUT);

    panServo.attach(9);   // D9
    tiltServo.attach(10); // D10

    Serial.begin(baudRate);

    frontLeftMotor->run(RELEASE); 
    rearLeftMotor->run(RELEASE); 
    frontRightMotor->run(RELEASE); 
    rearRightMotor->run(RELEASE); 

    frontLeftMotor->setSpeed(0);
    rearLeftMotor->setSpeed(0);
    frontRightMotor->setSpeed(0);
    rearRightMotor->setSpeed(0);

    // 100 is about centerline
    panServo.write(100);
    // 200 is straight up and down
    // If it's aimed too far upwards, the lights in the ceiling mess with the
    // camera's exposure settings
    tiltServo.write(160);
}

void serialEvent()
{
    uint8_t inputBuf[messageSize];
    if (Serial.readBytes(inputBuf, messageSize) != messageSize)
        // Dropped some data, so ignore the message
        return;
    
    MessageId_t id = static_cast<MessageId_t>(inputBuf[0]);
    switch(id)
    {
        case MessageId_t::SERVO_CONTROL:
            handleServoControl(inputBuf);
        break;
        case MessageId_t::WHEEL_CONTROL:
            handleWheelControl(inputBuf);
        break;
        case MessageId_t::ULTRASONIC_INFO:
            handleUltrasonicRequest(inputBuf);
        break;
        default:
            // Could not parse message
        break;
    }
}

void loop()
{
    // Delay 100ms if we don't have serial input
    delay(100);
}

void handleServoControl(const uint8_t* msg)
{
    uint8_t panServoAngle = servoControl::deserializePanServoAngle(msg);
    if (panServoAngle != InvalidServoAngle)
        panServo.write(panServoAngle);

    uint8_t tiltServoAngle = servoControl::deserializeTiltServoAngle(msg);
    if (tiltServoAngle != InvalidServoAngle)
        tiltServo.write(tiltServoAngle);
}

void handleWheelControl(const uint8_t* msg)
{
    uint8_t wheelSpeed = wheelControl::deserializeWheelSpeed(msg);
    if (wheelSpeed != InvalidWheelSpeed)
    {
        // Adjust the PWM speed
        // Clamped from 0-255 (unsigned so always positive)
        wheelSpeed = max(255, wheelSpeed);
        frontRightMotor->setSpeed(wheelSpeed);
        frontLeftMotor->setSpeed(wheelSpeed);
        rearRightMotor->setSpeed(wheelSpeed);
        rearLeftMotor->setSpeed(wheelSpeed);
    }

    MotorDir_t leftSideDir  = MotorDir_t::M_INVALID_DIR;
    MotorDir_t rightSideDir = MotorDir_t::M_INVALID_DIR;

    wheelControl::deserializeWheelDirs(msg, &leftSideDir, &rightSideDir);
    
    if (leftSideDir != MotorDir_t::M_INVALID_DIR && rightSideDir != MotorDir_t::M_INVALID_DIR)
    {
        frontLeftMotor->run(static_cast<uint8_t>(leftSideDir)); 
        rearLeftMotor->run(static_cast<uint8_t>(leftSideDir)); 
        frontLeftMotor->run(static_cast<uint8_t>(rightSideDir)); 
        rearLeftMotor->run(static_cast<uint8_t>(rightSideDir)); 
    }
}

void handleUltrasonicRequest(const uint8_t* msg)
{
    static uint32_t ultrasonicTimestamp_ms = 0;
    static uint32_t distance_cm = 0;

    // Trigger read of ultrasonic sensor, ensure it's been at least 60
    // ms since the last reading
    if (uint32_t now_ms = millis(); now_ms - ultrasonicTimestamp_ms > 60)
    {
        // It's been at least 60 ms since the last reading, update the distance
        distance_cm = readUltrasonicSensor();
        ultrasonicTimestamp_ms = now_ms;
    }

    // Respond to request regardless if the value is new or old
    ultrasonicInfo::serializeDistance(distance_cm, msg);
    Serial.write(msg, messageSize);
}

uint32_t readUltrasonicSensor()
{
    // Pull trigger high for at lesat 10 microseconds, then low
    digitalWrite(UltrasonicTriggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(UltrasonicTriggerPin, LOW);

    while (digitalRead(UltrasonicEchoPin) == 0)
    {
        // Wait until the echo pin goes high
    }
    
    // Get wall clock time in microseconds
    uint32_t start_us = micros();

    while (digitalRead(UltrasonicEchoPin) == 1)
    {
        // Wait until the echo pin goes low again
    }

    // Find difference between start and now
    uint32_t pulseLength_us = micros() - start_us;

    // Calculate the distance in centimeters
    // Assumes that the speed of sound when at sea level (340 m/s)
    uint32_t dist_cm = pulseLength_us / 58;
    return dist_cm;
}
