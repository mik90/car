#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <Wire.h>

#include "netUtility.hpp"
#include "wifiInfo.h"

int status = WL_IDLE_STATUS;
WiFiServer server(80);
constexpr int myI2cAddress = 1;
constexpr int motorControllerAddress = 2;

void setup() {
    Wire.begin(myI2cAddress);
    Serial.begin(9600);
    while (!Serial) {
        // Wait until serial port is connected
    }

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

        // Wait 10 seconds to connect
        delay(10000);
    }
    Serial.print("Connected.");
    printWifiData();
    server.begin();
    Serial.print("Started server.");
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {
    Serial.println("Client available");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        const char c = client.read();
        Serial.write(c);
        if (c == '\n') {

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> to make the car go<br>");
            client.print("Click <a href=\"/L\">here</a> to make the car stop<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          Wire.beginTransmission(motorControllerAddress);
          Wire.write(150); // Speed
          Wire.write(1); // Left dir
          Wire.write(1); // Right dir
          Wire.endTransmission();
        }
        if (currentLine.endsWith("GET /L")) {
          Wire.beginTransmission(motorControllerAddress);
          Wire.write(0); // Speed
          Wire.write(0); // Left dir
          Wire.write(0); // Right dir
          Wire.endTransmission();
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

