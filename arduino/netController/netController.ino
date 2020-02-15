#include <Arduino.h>
#include <SPI.h>
#include <WiFiNINA.h>

#include "wifiInfo.h"

int status = WL_IDLE_STATUS;

void setup() {
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

    Serial.println("Connected.");
    printCurrentNet();
    printWifiData();
}

void loop() {
  // check the network connection once every 10 seconds:
  delay(10000);
  printCurrentNet();
}

void printWifiData() {
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}