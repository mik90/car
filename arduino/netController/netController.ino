#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <Wire.h>

#include "netUtility.hpp"
#include "wifiInfo.h"

// From Adafruit_MotorShield.h
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

int status = WL_IDLE_STATUS;
WiFiUDP Udp;
constexpr int localPort = 50001;

char packetBuffer[255];

void setup() {
    Serial.begin(9600);
    
    if (WiFi.status() == WL_NO_MODULE) {
        Serial.println("Could not communicate with WiFi Nina module");
        while(true);
    }

    String fv = WiFi.firmwareVersion();
    if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
        Serial.println("Please upgrade the WiFi module firmware");
    }
    

    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to WPA2 Personal SSID:");
        Serial.println(ssid);

        status = WiFi.begin(ssid, pass);

        //Wait 10 seconds to connect
        delay(10000);
    }
    Serial.print("Connected.");
    printWifiData();
    Udp.begin(localPort);
    // RX and TX1
    Serial1.begin(9600);
    Serial.print("Started server."); 
}
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
    const int packetSize = Udp.parsePacket();
    if (packetSize > 0) {

        Serial.println("Received UDP packet of size:" + String(packetSize));
        //Serial.print("from " + String(Udp.remoteIP()) + ":" + String(Udp.remotePort()) + "\n");


        const int bytesRead = Udp.read(packetBuffer, 255);
        if (bytesRead > 0) {
            // Ensure that the buffer is null-terminated
            packetBuffer[bytesRead] = '\0';
        }
        const String json(packetBuffer);

        //Serial.println("Message:" + json);
        StaticJsonDocument<200> doc;
        //Serial.println("Allocated Doc");
        DeserializationError error = deserializeJson(doc, json);
        //Serial.println("Attempted deserialization");

        if (error) {
            Serial.print("Could not deserialize JSON. ");
            Serial.println(error.c_str());
            return;
        }
        Serial.println("No error");

        String speed = doc["wheel_speed"];
        String left_side_dir = doc["left_side_dir"];
        String right_side_dir = doc["right_side_dir"];


        Serial.println("Beginning transmission");
        uint8_t speedBuf = speed.toInt();
        //Serial.println("Writing:" + speed);
        Serial1.write(speedBuf);
        Serial1.write(',');

        uint8_t leftBuf = stringToCommand(left_side_dir);
        //Serial.println("Writing:" + left_side_dir);
        Serial1.write(leftBuf);
        Serial1.write(',');

        uint8_t rightBuf = stringToCommand(right_side_dir);
        //Serial.println("Writing:" + right_side_dir);
        Serial1.write(rightBuf);
        //Serial.println("Writing null char");
        Serial1.write("\0");
        //Serial.println("Wrote null char");
        
       Serial.println("Send to wire:" + speed + "," 
                       + left_side_dir + "," + right_side_dir + "\0");
    }
  delay(100);

}