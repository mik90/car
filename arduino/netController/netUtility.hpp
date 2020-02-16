#ifndef NET_UTILITY_HPP_
#define NET_UTILITY_HPP_

#include <SPI.h>
#include <WiFiNINA.h>

void printCurrentNet();
void printWifiData();
void printMacAddress(byte mac[]);

#endif
