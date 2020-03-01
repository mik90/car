#ifndef MOTOR_CONTROLLER_HPP_
#define MOTOR_CONTROLLER_HPP_

#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>


namespace mik {

class MotorController {
    public:
    MotorController();
    void init(int softSerialBaudRate);
    String monitorSoftwareSerialPort();

    private:
    void setMotorSpeeds(int speed);
    void setLeftSideDir(int dir);
    void setRightSideDir(int dir);
    long readVcc();

    Adafruit_MotorShield m_shield;
    SoftwareSerial       m_controlSerial;
    Adafruit_DCMotor*    m_frontRightMotor;
    Adafruit_DCMotor*    m_frontLeftMotor;
    Adafruit_DCMotor*    m_rearRightMotor;
    Adafruit_DCMotor*    m_rearLeftMotor;
};

}

#endif