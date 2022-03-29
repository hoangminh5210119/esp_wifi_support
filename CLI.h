#ifndef CLI_h
#define CLI_h

#include "Arduino.h"
#include "LittleFS.h"
#include <ESP8266WiFi.h>
#include <FS.h>
extern "C" {
#include "user_interface.h"
}
#include "SimpleList.h"
#include "language.h"
#include <EEPROM.h>

extern uint32_t currentTime;
extern uint32_t autosaveTime;

extern String macToStr(uint8_t *mac);
extern void strToColor(String str, uint8_t *buf);
extern void readFileToSerial(String path, bool showLineNum);
extern bool readFile(String path, String &buf);
extern bool removeFile(String path);
extern bool copyFile(String pathFrom, String pathTo);
extern bool renameFile(String pathFrom, String pathTo);
extern bool appendFile(String path, String &buf);
extern bool removeLines(String path, int lineFrom, int lineTo);
extern bool replaceLine(String path, int line, String &buf);
extern bool equalsKeyword(const char *str, const char *keyword);
extern void startAP(String path, String ssid, String password, uint8_t ch,
                    bool hidden, bool captivePortal, bool none_password);
extern void wifiUpdate();

class CLI {
public:
  CLI();
  ~CLI();

  void load();
  void load(String filepath);

  void enable();
  void disable();

  void check_boot_reset_cnt(int reset_cnt = 5);
  void reset_boot_reset_cnt();

  void update();
  void stop();

  void enableDelay(uint32_t delayTime);

  void exec(String input);
  void execFile(String path);
  void runLine(String input);
  void runCommand(String input);

private:
  bool enabled = true;

  bool booted = false;

  SimpleList<String> *list;
  SimpleList<String> *queue;

  bool delayed = false;
  uint32_t delayTime = 0;
  uint32_t delayStartTime = 0;

  String execPath = "/autostart.txt";

  struct Keyword {
    const char *name;
    const char *shortName;
    const char *alt;
  };

  void error(String message);
  void parameterError(String parameter);
  bool isInt(String str);
  int toInt(String str);
  uint32_t getTime(String time);
  bool eqlsCMD(int i, const char *keyword);
};

#endif // ifndef CLI_h