
#include "Arduino.h"
#include "CLI.h"
#include "DNSServer.h"
#include "ESP8266WebServer.h"
#include "ESP8266WiFi.h"
#include "LittleFS.h"
#include "SoftwareSerial.h"
#include <ArduinoJson.h>
#include <BlynkSimpleEsp8266.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

CLI cli;


String auth = "";

// Your WiFi credentials.
// Set password to "" for open networks.
String blynk_host = "blynk-cloud.com";
int blynk_port = 80;


void load_file_system();
void save_file_system();
void config_wifi();
WiFiManagerParameter blynk_auth_param("auth", "auth token", "", 40);
WiFiManagerParameter blynk_server_param("server", "blynk server", "blynk-cloud.com", 40);
                                        
WiFiManagerParameter blynk_port_param("port", "blynk port", "80", 6);
WiFiManager wifiManager;


void setup() {
  Serial.begin(115200);
  // delay(2000);

  if (cli.check_boot_reset_cnt()) {
    config_wifi();
  }

  delay(2000);
  load_file_system();
  Serial.println("Connecting to WiFi");
  Blynk.begin(auth.c_str(), WiFi.SSID().c_str(), WiFi.psk().c_str(), blynk_host.c_str(), String(blynk_port).toInt());
  Serial.println("started");


  // String a = "12";
  // writeFile("/data/config.txt", a);
}

void loop() {
  Blynk.run();
  cli.update();
  // wifiManager.process();
}

void config_wifi() {
  wifiManager.resetSettings();
  wifiManager.addParameter(&blynk_auth_param);
  wifiManager.addParameter(&blynk_server_param);
  wifiManager.addParameter(&blynk_port_param);
  if (!wifiManager.autoConnect("ESP_WIFI")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    // reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }
  save_file_system();
}

void load_file_system() {
  if (LittleFS.exists("/config.json")) {
    File configFile = LittleFS.open("/config.json", "r");
    if (configFile) {
      size_t size = configFile.size();
      std::unique_ptr<char[]> buf(new char[size]);
      configFile.readBytes(buf.get(), size);
      DynamicJsonDocument json(200);
      auto deserializeError = deserializeJson(json, buf.get());
      serializeJson(json, Serial);
      auth = json["auth"].as<String>();
      blynk_host = json["blynk_host"].as<String>();
      blynk_port = json["blynk_port"].as<int>();
    }
      configFile.close();
  }
}

void save_file_system() {
  String __auth = blynk_auth_param.getValue();
  String __blynk_host = blynk_server_param.getValue();
  String __blynk_port = blynk_port_param.getValue();
  Serial.println("The values in the file are: ");
  Serial.println("auth : " + String(__auth));
  Serial.println("blynk_host : " + String(__blynk_host));
  Serial.println("blynk_port : " + String(__blynk_port));

  DynamicJsonDocument json(200);
  json["auth"] = __auth;
  json["blynk_host"] = __blynk_host;
  json["blynk_port"] = __blynk_port;
  File configFile = LittleFS.open("/config.json", "w");
  serializeJson(json, Serial);
  serializeJson(json, configFile);
  configFile.close();
}