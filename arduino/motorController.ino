#include <Serial.h>
#include <Servo.h>
#include <Adafruit_MotorShield.h>

#ifndef ARDUINO
#define ARDUINO
#endif
#include "messages.ino"

Adafruit_MotorShield shield;
Servo panServo;
Servo tiltServo;
constexpr int M1 = 1;
constexpr int M2 = 2;
constexpr int M3 = 3;
constexpr int M4 = 4;
constexpr int UltrasonicEchoPin = 7; // D7
constexpr int UltrasonicTriggerPin = 8; // D8
constexpr int D9 = 9;
constexpr int D10 = 10;

auto frontRightMotor = shield.getMotor(M1);
auto frontLeftMotor  = shield.getMotor(M2);
auto rearRightMotor  = shield.getMotor(M3);
auto rearLeftMotor   = shield.getMotor(M4);
void handleWheelControl(const std::array<uint8_t,messageSize>& msg);
void handleServoControl(const std::array<uint8_t,messageSize>& msg);
void handleServoRequest(const std::array<uint8_t,messageSize>& msg);
uint32_t readUltrasonicSensor();

void setup()
{
    shield.begin();

    pinMode(UltrasonicEchoPin, INPUT);
    pinMode(UltrasonicTriggerPin, OUTPUT);

    panServo.attach(D9);
    tiltServo.attach(D10);
    
    frontRightMotor->run(RELEASE);
    frontLeftMotor->run(RELEASE); 
    rearRightMotor->run(RELEASE); 
    rearLeftMotor->run(RELEASE); 

    Serial.begin(baudRate);
    while (!Serial)
    {
        // Wait for serial port to be available
        delayMicroseconds(100);
    }
}

void serialEvent()
{
    if (Serial.available() >= messageSize)
    {
        std::array<uint8_t, messageSize> inputBuf;
        if (Serial.readBytes(inputBuf.data(), messageSize) != messageSize)
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
}

void loop()
{
    // Only act during serial input
    delay(100);
}

void handleServoControl(const std::array<uint8_t,messageSize>& msg)
{
    uint8_t panServoAngle = servoControl::deserializePanServoAngle(msg);
    if (panServoAngle != InvalidServoAngle)
    {
        // Clamped from 0-100 (unsigned so always positive)
        panServoAngle = max(100, panServoAngle);
        panServo.write(panServoAngle);
    }

    uint8_t tiltServoAngle = servoControl::deserializeTiltServoAngle(msg);
    if (tiltServoAngle != InvalidServoAngle)
    {
        tiltServoAngle = max(100, tiltServoAngle);
        tiltServo.write(tiltServoAngle);
    }
}

void handleWheelControl(const std::array<uint8_t,messageSize>& msg)
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

    auto leftAndRightDir = wheelControl::deserializeWheelDirs(msg);
    MotorDir_t leftSideDir = leftAndRightDir.first;
    if (leftSideDir != MotorDir_t::INVALID_DIR)
    {
        frontLeftMotor->run(static_cast<uint8_t>(leftSideDir)); 
        rearLeftMotor->run(static_cast<uint8_t>(leftSideDir)); 
    }

    MotorDir_t rightSideDir = leftAndRightDir.second;
    if (rightSideDir != MotorDir_t::INVALID_DIR)
    {
        frontLeftMotor->run(static_cast<uint8_t>(rightSideDir)); 
        rearLeftMotor->run(static_cast<uint8_t>(rightSideDir)); 
    }
}

void handleUltrasonicRequest(const std::array<uint8_t,messageSize>& msg)
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
    auto outputAr = ultrasonicInfo::serializeDistance(distance_cm);
    Serial.write(outputAr.data(), outputAr.size());
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