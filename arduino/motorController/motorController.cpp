#include "motorController.hpp"


namespace mik {

MotorController::MotorController() : m_shield(),
                                     m_controlSerial(2, 3) // RX, TX Digital Pins
                                    {
    m_frontRightMotor = m_shield.getMotor(1); // M1
    m_frontLeftMotor  = m_shield.getMotor(2); // M2
    m_rearRightMotor  = m_shield.getMotor(3); // M3
    m_rearLeftMotor   = m_shield.getMotor(4); // M4
}
void MotorController::init(int softSerialBaudRate) {
    m_shield.begin();
    this->setMotorSpeeds(0);
    m_controlSerial.begin(softSerialBaudRate);
}

String MotorController::monitorSoftwareSerialPort() {
    const String newline("\r\n");
    // Example data: 150,0,0\0
    String status;
    if (m_controlSerial.available() > 0) {
        status += "Received " + String(m_controlSerial.available()) + " bytes" + newline;
        // 0 - 255
        uint8_t speed = m_controlSerial.read();
        m_controlSerial.read(); // Discard comma

        uint8_t left_side_dir = m_controlSerial.read();
        m_controlSerial.read(); // Discard comma

        uint8_t right_side_dir = m_controlSerial.read();
        m_controlSerial.read(); // Discard comma
        
        status += "Received control data: " + String(speed) + "," 
                                            + String(left_side_dir) + ","
                                            + String(right_side_dir) + newline;
        this->setMotorSpeeds(speed);
        this->setLeftSideDir(left_side_dir);
        this->setRightSideDir(right_side_dir);
        status += "Sent commands to motors" + newline;
        status += "Current input voltage is " + String(this->readVcc()) + " mV" + newline;
    }

    return status;
}

void MotorController::setMotorSpeeds(int speed) {
    m_frontLeftMotor->setSpeed(speed);
    m_rearLeftMotor->setSpeed(speed);
    m_frontRightMotor->setSpeed(speed);
    m_rearRightMotor->setSpeed(speed);
}

void MotorController::setLeftSideDir(int dir) {
    m_frontLeftMotor->run(dir); 
    m_rearLeftMotor->run(dir); 
}

void MotorController::setRightSideDir(int dir) {
    m_frontRightMotor->run(dir); 
    m_rearRightMotor->run(dir); 
}

// Source https://code.google.com/archive/p/tinkerit/wikis/SecretVoltmeter.wiki
long MotorController::readVcc() { 
    long result;
    // Read 1.1V reference against AVcc
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    delay(2); // Wait for Vref to settle
    ADCSRA |= _BV(ADSC); // Convert
    while (bit_is_set(ADCSRA,ADSC)) {
        // Wait
    };
    result = ADCL;
    result |= ADCH<<8;
    result = 1126400L / result; // Back-calculate AVcc in mV
    return result;
}


}
