#include "CLI.h"
#include "functions.h"

/*
   Shitty code used less resources so I will keep this clusterfuck as it is,
   but if you're interested I made a library for this:
   github.com/spacehuhn/SimpleCLI
 */

CLI::CLI() {
  list = new SimpleList<String>;
  queue = new SimpleList<String>;
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed");
    LittleFS.format();
    return;
  }
}

CLI::~CLI() {}

void CLI::reset_boot_reset_cnt() {
  if (!booted) {
    // reset boot counter
    EEPROM.write(0, 0);
    EEPROM.commit();
    booted = true;
  }
}

void CLI::save_file(String path, String &buf) { writeFile(path, buf); }
void CLI::read_file(String path, String &buf) { readFile(path, buf); }

bool CLI::check_boot_reset_cnt(int reset_cnt) {
  EEPROM.begin(4096);
  uint8_t bootCounter = EEPROM.read(0);

  if (bootCounter >= reset_cnt) {
    Serial.println("SETUP_FORMAT_SPIFFS");
    LittleFS.format();
    Serial.println("SETUP_OK");
    return true;
  } else {
    EEPROM.write(0, bootCounter + 1); // add 1 to the boot counter
    EEPROM.commit();
  }
  return false;
}

void CLI::load() {
  String defaultValue = str(CLI_DEFAULT_AUTOSTART);
  checkFile(execPath, defaultValue);
  execFile(execPath);
}

void CLI::load(String filepath) {
  execPath = filepath;
  load();
}

void CLI::enable() {
  enabled = true;
  prntln(CLI_SERIAL_ENABLED);
}

void CLI::disable() {
  enabled = false;
  prntln(CLI_SERIAL_DISABLED);
}

void CLI::update() {
  // when serial available, read input
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    exec(input);
  }
  reset_boot_reset_cnt();
}

void CLI::stop() {
  queue->clear();
  prntln(CLI_STOPPED_SCRIPT);
}

void CLI::enableDelay(uint32_t delayTime) {
  delayed = true;
  this->delayTime = delayTime;
  delayStartTime = millis();
}

void CLI::exec(String input) {
  // quick exit when input is empty
  if (input.length() == 0)
    return;

  // check delay
  if (delayed && (millis() - delayStartTime > delayTime)) {
    delayed = false;
  }

  // when delay is on, add it to queue, else run it
  if (delayed) {
    queue->add(input);
  } else {
    runLine(input);
  }
}

void CLI::execFile(String path) {
  String input;

  if (readFile(path, input)) {
    String tmpLine;
    char tmpChar;

    input += '\n';

    while (!queue->isEmpty()) {
      input += queue->shift();
      input += '\n';
    }

    for (int i = 0; i < input.length(); i++) {
      tmpChar = input.charAt(i);

      if (tmpChar == '\n') {
        queue->add(tmpLine);
        tmpLine = String();
      } else {
        tmpLine += tmpChar;
      }
    }

    queue->add(tmpLine);
  }
}

void CLI::error(String message) {
  prnt(CLI_ERROR);
  prntln(message);
}

void CLI::parameterError(String parameter) {
  prnt(CLI_ERROR_PARAMETER);
  prnt(parameter);
  prntln(DOUBLEQUOTES);
}

bool CLI::isInt(String str) {
  if (eqls(str, STR_TRUE) || eqls(str, STR_FALSE))
    return true;

  for (uint32_t i = 0; i < str.length(); i++)
    if (!isDigit(str.charAt(i)))
      return false;

  return true;
}

int CLI::toInt(String str) {
  if (eqls(str, STR_TRUE))
    return 1;
  else if (eqls(str, STR_FALSE))
    return 0;
  else
    return str.toInt();
}

uint32_t CLI::getTime(String time) {
  int value = time.toInt();

  if (value < 0)
    value = -value;

  if (time.substring(time.length() - 1).equalsIgnoreCase(String(S)))
    value *= 1000;
  else if (time.substring(time.length() - 3).equalsIgnoreCase(str(STR_MIN)) ||
           (time.charAt(time.length() - 1) == M))
    value *= 60000;
  return value;
}

bool CLI::eqlsCMD(int i, const char *keyword) {
  return eqls(list->get(i).c_str(), keyword);
}

void CLI::runLine(String input) {
  String tmp;

  for (int i = 0; i < input.length(); i++) {
    // when 2 semicolons in a row without a backslash escaping the first
    if ((input.charAt(i) == SEMICOLON) && (input.charAt(i + 1) == SEMICOLON) &&
        (input.charAt(i - 1) != BACKSLASH)) {
      runCommand(tmp);
      tmp = String();
      i++;
    } else {
      tmp += input.charAt(i);
    }
  }

  tmp.replace(BACKSLASH + SEMICOLON + SEMICOLON, SEMICOLON + SEMICOLON);

  if (tmp.length() > 0)
    runCommand(tmp);
}

void CLI::runCommand(String input) {
  input.replace(String(NEWLINE), String());
  input.replace(String(CARRIAGERETURN), String());

  list->clear();

  // parse/split input in list
  String tmp;
  bool withinQuotes = false;
  bool escaped = false;
  char c;

  for (uint32_t i = 0; i < input.length() && i < 512; i++) {
    c = input.charAt(i);

    // when char is an unescaped
    if (!escaped && (c == BACKSLASH)) {
      escaped = true;
    }

    // (when char is a unescaped space AND it's not within quotes) OR char is \r
    // or \n
    else if (((c == SPACE) && !escaped && !withinQuotes) ||
             (c == CARRIAGERETURN) || (c == NEWLINE)) {
      // when tmp string isn't empty, add it to the list
      if (tmp.length() > 0) {
        list->add(tmp);
        tmp = String(); // reset tmp string
      }
    }

    // when char is an unescaped "
    else if ((c == DOUBLEQUOTES) && !escaped) {
      // update wheter or not the following chars are within quotes or not
      withinQuotes = !withinQuotes;

      if ((tmp.length() == 0) && !withinQuotes)
        tmp += SPACE; // when exiting quotes and tmp string is empty, add
                      // a space
    }

    // add character to tmp string
    else {
      tmp += c;
      escaped = false;
    }
  }

  // add string if something is left from the loop above
  if (tmp.length() > 0)
    list->add(tmp);

  // stop when input is empty/invalid
  if (list->size() == 0)
    return;

  // print comments
  if (list->get(0) == str(CLI_COMMENT)) {
    prntln(input);
    return;
  }

  if (list->size() == 0)
    return;

  // ===== HELP ===== //
  if (eqlsCMD(0, CLI_HELP)) {
    prntln(CLI_HELP_HEADER);

    prntln(CLI_HELP_SYSINFO);
    prntln(CLI_HELP_CLEAR);
    prntln(CLI_HELP_FORMAT);
    prntln(CLI_HELP_PRINT);
    prntln(CLI_HELP_DELETE);
    prntln(CLI_HELP_REPLACE);
    prntln(CLI_HELP_COPY);
    prntln(CLI_HELP_RENAME);
    prntln(CLI_HELP_RUN);
    prntln(CLI_HELP_WRITE);
    prntln(CLI_HELP_INFO);
    prntln(CLI_HELP_COMMENT);

  }

  // ===== SYSTEM ===== //
  // sysinfo
  else if (eqlsCMD(0, CLI_SYSINFO)) {
    prntln(CLI_SYSTEM_INFO);
    char s[150];
    sprintf(s, str(CLI_SYSTEM_OUTPUT).c_str(),
            81920 - system_get_free_heap_size(),
            100 - system_get_free_heap_size() / (81920 / 100),
            system_get_free_heap_size(),
            system_get_free_heap_size() / (81920 / 100), 81920);
    prntln(String(s));

    uint8_t mac[6];

    prnt(CLI_SYSTEM_AP_MAC);
    wifi_get_macaddr(SOFTAP_IF, mac);
    prntln(macToStr(mac));

    prnt(CLI_SYSTEM_ST_MAC);
    wifi_get_macaddr(STATION_IF, mac);
    prntln(macToStr(mac));

    FSInfo fs_info;
    LittleFS.info(fs_info);
    sprintf(s, str(CLI_SYSTEM_RAM_OUT).c_str(), fs_info.usedBytes,
            fs_info.usedBytes / (fs_info.totalBytes / 100),
            fs_info.totalBytes - fs_info.usedBytes,
            (fs_info.totalBytes - fs_info.usedBytes) /
                (fs_info.totalBytes / 100),
            fs_info.totalBytes);
    prnt(String(s));
    sprintf(s, str(CLI_SYSTEM_SPIFFS_OUT).c_str(), fs_info.blockSize,
            fs_info.pageSize);
    prnt(String(s));
    prntln(CLI_FILES);
    Dir dir = LittleFS.openDir(String(SLASH));

    while (dir.next()) {
      prnt(String(SPACE) + String(SPACE) + dir.fileName() + String(SPACE));
      File f = dir.openFile("r");
      prnt(int(f.size()));
      prntln(str(CLI_BYTES));
    }
    prntln(CLI_SYSTEM_FOOTER);
  }

  // ===== CLEAR ===== //
  // clear
  else if (eqlsCMD(0, CLI_CLEAR)) {
    for (int i = 0; i < 100; i++)
      prnt(HASHSIGN);

    for (int i = 0; i < 60; i++)
      prntln();
  }

  // ===== REBOOT ===== //
  // reboot
  else if (eqlsCMD(0, CLI_REBOOT)) {
    ESP.reset();
  }

  // ===== FORMAT ==== //
  // format
  else if (eqlsCMD(0, CLI_FORMAT)) {
    prnt(CLI_FORMATTING_SPIFFS);
    LittleFS.format();
    prntln(SETUP_OK);
  }

  // ===== DELETE ==== //
  // delete <file> [<lineFrom>] [<lineTo>]
  else if ((list->size() >= 2) && eqlsCMD(0, CLI_DELETE)) {
    if (list->size() == 2) {
      // remove whole file
      if (removeFile(list->get(1))) {
        prnt(CLI_REMOVED);
        prntln(list->get(1));
      } else {
        prnt(CLI_ERROR_REMOVING);
        prntln(list->get(1));
      }
    } else {
      // remove certain lines
      int beginLine = list->get(2).toInt();
      int endLine = list->size() == 4 ? list->get(3).toInt() : beginLine;

      if (removeLines(list->get(1), beginLine, endLine)) {
        prnt(CLI_REMOVING_LINES);
        prnt(beginLine);
        prnt(String(SPACE) + String(DASH) + String(SPACE));
        prnt(endLine);
        prntln(String(SPACE) + list->get(1));
      } else {
        prnt(CLI_ERROR_REMOVING);
        prntln(list->get(1));
      }
    }
  }

  // ===== COPY ==== //
  // delete <file> <newfile>
  else if ((list->size() == 3) && eqlsCMD(0, CLI_COPY)) {
    if (copyFile(list->get(1), list->get(2))) {
      prntln(CLI_COPIED_FILES);
    } else {
      prntln(CLI_ERROR_COPYING);
    }
  }

  // ===== RENAME ==== //
  // delete <file> <newfile>
  else if ((list->size() == 3) && eqlsCMD(0, CLI_RENAME)) {
    if (renameFile(list->get(1), list->get(2))) {
      prntln(CLI_RENAMED_FILE);
    } else {
      prntln(CLI_ERROR_RENAMING_FILE);
    }
  }

  // ===== WRITE ==== //
  // write <file> <commands>
  else if ((list->size() >= 3) && eqlsCMD(0, CLI_WRITE)) {
    String path = list->get(1);
    String buf = String();

    int listSize = list->size();

    for (int i = 2; i < listSize; i++) {
      buf += list->get(i);

      if (i < listSize - 1)
        buf += SPACE;
    }

    prnt(CLI_WRITTEN);
    prnt(buf);
    prnt(CLI_TO);
    prntln(list->get(1));

    buf += NEWLINE;
    appendFile(path, buf);
  }

  // ===== REPLACE ==== //
  // replace <file> <line> <new-content>
  else if ((list->size() >= 4) && eqlsCMD(0, CLI_REPLACE)) {
    int line = list->get(2).toInt();
    String tmp = String();

    for (int i = 3; i < list->size(); i++) {
      tmp += list->get(i);

      if (i < list->size() - 1)
        tmp += SPACE;
    }

    if (replaceLine(list->get(1), line, tmp)) {
      prnt(CLI_REPLACED_LINE);
      prnt(line);
      prnt(CLI_WITH);
      prntln(list->get(1));
    } else {
      prnt(CLI_ERROR_REPLACING_LINE);
      prntln(list->get(1));
    }
  }

  // ===== RUN ==== //
  // run <file> [continue <num>]
  else if ((list->size() >= 2) && eqlsCMD(0, CLI_RUN)) {
    execFile(list->get(1));
  }

  // ===== PRINT ==== //
  // print <file> [<lines>]
  else if ((list->size() >= 2) && eqlsCMD(0, CLI_PRINT)) {
    readFileToSerial(list->get(1), eqlsCMD(2, CLI_LINE));
    prntln();
  }

  // ===== INFO ===== //
  // info
  else if (eqlsCMD(0, CLI_INFO)) {
    prntln(CLI_INFO_HEADER);
    prntln(CLI_INFO_HEADER);
  }

  // // ===== SEND ===== //
  // // send deauth <apMac> <stMac> <rason> <channel>
  // else if (eqlsCMD(0, CLI_SEND) && (list->size() == 6) &&
  //          eqlsCMD(1, CLI_DEAUTH)) {
  //   uint8_t apMac[6];
  //   uint8_t stMac[6];
  //   strToMac(list->get(2), apMac);
  //   strToMac(list->get(3), stMac);
  //   uint8_t reason = list->get(4).toInt();
  //   uint8_t channel = list->get(5).toInt();
  //   prnt(CLI_DEAUTHING);
  //   prnt(macToStr(apMac));
  //   prnt(CLI_ARROW);
  //   prntln(macToStr(stMac));
  //   attack.deauthDevice(apMac, stMac, reason, channel);
  // }

  // // send beacon <mac> <ssid> <ch> [wpa2]
  // else if (eqlsCMD(0, CLI_SEND) && (list->size() >= 5) &&
  //          eqlsCMD(1, CLI_BEACON)) {
  //   uint8_t mac[6];
  //   strToMac(list->get(2), mac);
  //   uint8_t channel = list->get(4).toInt();
  //   String ssid = list->get(3);

  //   for (int i = ssid.length(); i < 32; i++)
  //     ssid += SPACE;
  //   prnt(CLI_SENDING_BEACON);
  //   prnt(list->get(3));
  //   prntln(DOUBLEQUOTES);
  //   attack.sendBeacon(mac, ssid.c_str(), channel, eqlsCMD(5, CLI_WPA2));
  // }

  // // send probe <mac> <ssid> <ch>
  // else if (eqlsCMD(0, CLI_SEND) && (list->size() == 5) &&
  //          eqlsCMD(1, CLI_PROBE)) {
  //   uint8_t mac[6];
  //   strToMac(list->get(2), mac);
  //   uint8_t channel = list->get(4).toInt();
  //   String ssid = list->get(3);

  //   for (int i = ssid.length(); i < 32; i++)
  //     ssid += SPACE;
  //   prnt(CLI_SENDING_PROBE);
  //   prnt(list->get(3));
  //   prntln(DOUBLEQUOTES);
  //   attack.sendProbe(mac, ssid.c_str(), channel);
  // }

  // ===== NOT FOUND ===== //
  else {
    prnt(CLI_ERROR_NOT_FOUND_A);
    prnt(input);
    prntln(CLI_ERROR_NOT_FOUND_B);
    // some debug stuff

    /*
       Serial.println(list->get(0));
       for(int i=0;i<input.length();i++){
       Serial.print(input.charAt(i), HEX);
       Serial.print(' ');
       }
     */
  }
}
