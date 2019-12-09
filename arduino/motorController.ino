#include <Wire.h>
#include <Servo.h>
#include <Adafruit_MotorShield.h>

#include <math.h>

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

    // Use address 8 on I2C bus
    Wire.begin(8);
    Wire.onReceive(handleI2cMessage);
    Wire.onRequest(handleI2cRequest);
}

void handleServoControl()
{
    unsigned int panServoAngle =  Wire.read();
    if (panServoAngle != InvalidServoAngle)
    {
        // Clamped from 0-100 (unsigned so always positive)
        uint8_t clampedAngle = max(100, panServoAngle);
        panServo.write(panServoAngle);
    }

    unsigned int tiltServoAngle = Wire.read();
    if (tiltServoAngle != InvalidServoAngle)
    {
        uint8_t clampedAngle = max(100, tiltServoAngle);
        tiltServo.write(tiltServoAngle);
    }
}

void handleWheelControl()
{
    unsigned int wheelSpeed = Wire.read();
    if (wheelSpeed != InvalidWheelSpeed)
    {
        // Adjust the PWM speed
        // Clamped from 0-255 (unsigned so always positive)
        uint8_t clampedSpeed = max(255, wheelSpeed);
        frontRightMotor->setSpeed(clampedSpeed);
        frontLeftMotor->setSpeed(clampedSpeed);
        rearRightMotor->setSpeed(clampedSpeed);
        rearLeftMotor->setSpeed(clampedSpeed);
    }

    MotorDir_t leftSideDir = Wire.read();
    frontLeftMotor->run(leftSideDir); 
    rearLeftMotor->run(leftSideDir); 

    MotorDir_t rightSideDir = Wire.read();
    frontRightMotor->run(rightSideDir);
    rearRightMotor->run(rightSideDir); 
}

unsigned long readUltrasonicSensor()
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
    unsigned long start_us = micros();

    while (digitalRead(UltrasonicEchoPin) == 1)
    {
        // Wait until the echo pin goes low again
    }

    // Find difference between start and now
    unsigned long pulseLength_us = micros() - start_us;

    // Calculate the distance in centimeters
    // Assumes that the speed of sound when at sea level (340 m/s)
    unsigned long dist_cm = pulseLength_us / 58;
}

void handleI2cRequest()
{
    static unsigned long ultrasonicTimestamp_ms = 0;
    static unsigned long distance_cm = 0;

    // Trigger read of ultrasonic sensor, ensure it's been at least 60
    // ms since the last reading
    if (unsigned long now_ms = millis(); now_ms - ultrasonicTimestamp_ms > 60)
    {
        // It's been at least 60 ms since the last reading, update the distance
        distance_cm = readUltrasonicSensor();
        ultrasonicTimestamp_ms = now_ms;
    }

    // Respond to request regardless if the value is new or old
    Wire.write(static_cast<uint8_t>(MessageId_t::ULTRASONIC_INFO));
    Wire.write(distance_cm);
    
}

void handleI2cMessage(int nBytes)
{
    if (Wire.available() > sizeof(MessageId_t))
    {
        // First byte is always the ID
        MessageId_t id = Wire.read();
        switch(id)
        {
            case MessageId_t::SERVO_CONTROL:
                handleServoControl();
            break;

            case MessageId_t::WHEEL_CONTROL:
                handleWheelControl();
            break;

            default:
                // Could not parse message
            break;
        }
    }
}

void loop()
{
    // Only act when receiving a messsage
    delay(100);
}
