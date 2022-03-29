# esp_wifi_support
support library for esp wifi 


## Sử dụng

```
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

```

## Tính năng
- Work with file system 
- delete, create, print file.
- ...

## Command

- help => print list of commands can use.

```
[===== List of commands =====]
sysinfo
clear
format
print <file> [<lines>]
delete <file> [<lineFrom>] [<lineTo>]
replace <file> <line> <new-content>
copy <file> <newfile>
rename <file> <newfile>
run <file>
write <file> <commands>
info
```