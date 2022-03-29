#include "Arduino.h"
#include "CLI.h"
#include "DNSServer.h"
#include "ESP8266WebServer.h"
#include "ESP8266WiFi.h"
#include "LittleFS.h"
#include <EEPROM.h>

CLI cli;


void setup() {
  Serial.begin(115200);
  cli.check_boot_reset_cnt();
  Serial.println("");
  Serial.println("CLI started");
}

void loop() {
  cli.update();
}
