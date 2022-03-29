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

  String data = "helllo";
  cli.save_file("/config.txt", data);
  String _data;
  cli.read_file("/config.txt", _data);

  Serial.print("_data: ");
  Serial.println(_data);
}

void loop() { cli.update(); }
