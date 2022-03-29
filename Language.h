#ifndef language_h
#define language_h

#include "Arduino.h"

extern String str(const char *ptr);
extern String keyword(const char *keywordPtr);
extern bool eqls(const char *str, const char *keywordPtr);
extern bool eqls(String str, const char *keywordPtr);
extern String b2s(bool input);
extern String b2a(bool input);
extern bool s2b(String input);
extern void prnt(String s);
extern void prnt(bool b);
extern void prnt(char c);
extern void prnt(const char *ptr);
extern void prnt(int i);
extern void prntln();
extern void prntln(String s);
extern void prntln(bool b);
extern void prntln(char c);
extern void prntln(const char *ptr);
extern void prntln(int i);

/*
   The following variables are the strings used for the serial interface,
   display interface and settings. The keywords for the serial CLI have a simple
   structure to save a bit of memory and CPU time:
   - every keyword has a unique string
   - / is used for optional characters, i.e. 'enable/d' makes 'enable' and
   'enabled'
   - , is used for seperations, i.e. 'select/ed,-s' makes 'select', 'selected'
   and '-s'
   - everything is in lowercase
 */

// ===== GLOBAL STRINGS ===== //

// Often used characters, therefor in the RAM
const char CURSOR = '|';
const char SPACE = ' ';
const char DOUBLEPOINT = ':';
const char EQUALS = '=';
const char HASHSIGN = '#';
const char ASTERIX = '*';
const char PERCENT = '%';
const char DASH = '-';
const char QUESTIONMARK = '?';
const char ZERO = '0';
const char S = 's';
const char M = 'm';
const char D = 'd';
const char DOUBLEQUOTES = '\"';
const char SLASH = '/';
const char NEWLINE = '\n';
const char CARRIAGERETURN = '\r';
const char SEMICOLON = ';';
const char BACKSLASH = '\\';
const char POINT = '.';
const char VERTICALBAR = '|';
const char COMMA = ',';
const char ENDOFLINE = '\0';
const char OPEN_BRACKET = '[';
const char CLOSE_BRACKET = ']';
const char OPEN_CURLY_BRACKET = '{';
const char CLOSE_CURLY_BRACKET = '}';

const char STR_TRUE[] PROGMEM = "true";
const char STR_FALSE[] PROGMEM = "false";
const char STR_MIN[] PROGMEM = "min";



// ===== SETUP ===== //
const char SETUP_OK[] PROGMEM = "OK";
const char SETUP_ERROR[] PROGMEM = "ERROR";
const char SETUP_MOUNT_SPIFFS[] PROGMEM = "Mounting SPIFFS...";
const char SETUP_FORMAT_SPIFFS[] PROGMEM = "Formatting SPIFFS...";
const char SETUP_SERIAL_WARNING[] PROGMEM = "Warning: Serial deactivated";
const char SETUP_STARTED[] PROGMEM = "STARTED! \\o/";
const char SETUP_COPYING[] PROGMEM = "Copying ";
const char SETUP_PROGMEM_TO_SPIFFS[] PROGMEM = " from PROGMEM to SPIFFS...";

// ===== SERIAL COMMAND LINE INTERFACE ===== //
const char CLI_REBOOT[] PROGMEM = "reboot";            // reboot
const char CLI_CLEAR[] PROGMEM = "clear";              // clear
const char CLI_SYSINFO[] PROGMEM = "sysinfo";          // sysinfo
const char CLI_RESET[] PROGMEM = "reset";              // reset
const char CLI_GET[] PROGMEM = "get";                  // get
const char CLI_INFO[] PROGMEM = "info";                // info
const char CLI_HELP[] PROGMEM = "help";                // help
const char CLI_FORMAT[] PROGMEM = "format";            // format
const char CLI_DELETE[] PROGMEM = "delete";            // delete
const char CLI_PRINT[] PROGMEM = "print";              // print
const char CLI_RUN[] PROGMEM = "run";                  // run
const char CLI_WRITE[] PROGMEM = "write";              // write
const char CLI_LINE[] PROGMEM = "line/s,-l";        // line, lines, -l
const char CLI_REPLACED_LINE[] PROGMEM = "Replaced line ";
const char CLI_RENAME[] PROGMEM = "rename";            // rename
const char CLI_COMMENT[] PROGMEM = "//";            // //
const char CLI_REPLACE[] PROGMEM = "replace";          // replace
const char CLI_COPY[] PROGMEM = "copy";                // copy
const char CLI_HELP_SYSINFO[] PROGMEM = "sysinfo";
const char CLI_HELP_CLEAR[] PROGMEM = "clear";
const char CLI_HELP_FORMAT[] PROGMEM = "format";
const char CLI_HELP_PRINT[] PROGMEM = "print <file> [<lines>]";
const char CLI_HELP_DELETE[] PROGMEM = "delete <file> [<lineFrom>] [<lineTo>]";
const char CLI_HELP_REPLACE[] PROGMEM = "replace <file> <line> <new-content>";
const char CLI_HELP_COPY[] PROGMEM = "copy <file> <newfile>";
const char CLI_HELP_RENAME[] PROGMEM = "rename <file> <newfile>";
const char CLI_HELP_RUN[] PROGMEM = "run <file>";
const char CLI_HELP_WRITE[] PROGMEM = "write <file> <commands>";
const char CLI_HELP_GET[] PROGMEM = "get <setting>";
const char CLI_HELP_SET[] PROGMEM = "set <setting> <value>";
const char CLI_HELP_RESET[] PROGMEM = "reset";
const char CLI_HELP_CHICKEN[] PROGMEM = "chicken";
const char CLI_HELP_REBOOT[] PROGMEM = "reboot";
const char CLI_HELP_INFO[] PROGMEM = "info";

const char CLI_HELP_COMMENT[] PROGMEM = "// <comments>";
const char CLI_INPUT_PREFIX[] PROGMEM = "# ";
const char CLI_SERIAL_ENABLED[] PROGMEM = "Serial interface enabled";
const char CLI_SERIAL_DISABLED[] PROGMEM = "Serial interface disabled";
const char CLI_ERROR[] PROGMEM = "ERROR: ";
const char CLI_ERROR_PARAMETER[] PROGMEM = "Error Invalid parameter \"";
const char CLI_STOPPED_SCRIPT[] PROGMEM = "Cleared CLI command queue";
const char CLI_CONTINUOUSLY[] PROGMEM = "continuously";
const char CLI_EXECUTING[] PROGMEM = "Executing ";
const char CLI_SCRIPT_DONE_CONTINUE[] PROGMEM =
    "Done executing script - type 'stop script' to end the continuous mode";
const char CLI_SCRIPT_DONE[] PROGMEM = "Done executing script";
const char CLI_HELP_HEADER[] PROGMEM = "[===== List of commands =====]";


const char CLI_SYSTEM_INFO[] PROGMEM = "[======== SYSTEM INFO ========]";
const char CLI_SYSTEM_OUTPUT[] PROGMEM =
    "RAM usage: %u bytes used [%d%%], %u bytes free [%d%%], %u bytes in "
    "total\r\n";
const char CLI_SYSTEM_AP_MAC[] PROGMEM = "AP MAC address: ";
const char CLI_SYSTEM_ST_MAC[] PROGMEM = "Station MAC address: ";
const char CLI_SYSTEM_RAM_OUT[] PROGMEM =
    "SPIFFS: %u bytes used [%d%%], %u bytes free [%d%%], %u bytes in total\r\n";
const char CLI_SYSTEM_SPIFFS_OUT[] PROGMEM =
    "        block size %u bytes, page size %u bytes\r\n";

const char CLI_FILES[] PROGMEM = "Files: ";
const char CLI_BYTES[] PROGMEM = " bytes";
const char CLI_SYSTEM_FOOTER[] PROGMEM = "===============================";
const char CLI_FORMATTING_SPIFFS[] PROGMEM = "Formatting SPIFFS...";
const char CLI_REMOVED[] PROGMEM = "Removed ";
const char CLI_ERROR_REMOVING[] PROGMEM = "ERROR: removing ";
const char CLI_REMOVING_LINES[] PROGMEM = "Removed lines ";
const char CLI_COPIED_FILES[] PROGMEM = "Copied file";
const char CLI_ERROR_COPYING[] PROGMEM = "ERROR: Copying file";
const char CLI_RENAMED_FILE[] PROGMEM = "Renamed file";
const char CLI_ERROR_RENAMING_FILE[] PROGMEM = "ERROR: Renaming file";
const char CLI_WRITTEN[] PROGMEM = "Written \"";
const char CLI_TO[] PROGMEM = "\" to ";
const char CLI_WITH[] PROGMEM = " with ";
const char CLI_ERROR_REPLACING_LINE[] PROGMEM = "ERROR: replacing line in ";
const char CLI_INFO_HEADER[] PROGMEM =
    "=========================================================================="
    "==========";
const char CLI_DEFAULT_AUTOSTART[] PROGMEM = "scan -t 5s\nsysinfo\n";
const char CLI_ERROR_NOT_FOUND_A[] PROGMEM = "ERROR: command \"";
const char CLI_ERROR_NOT_FOUND_B[] PROGMEM = "\" not found :(";

// WIFI Mode
const char D_WIFI_MODE_AP[] PROGMEM = "AP";
const char D_WIFI_MODE_AP_STA[] PROGMEM = "AP_STA";
const char D_WIFI_MODE_STA[] PROGMEM = "STA";
const char D_WIFI_MODE_OFF[] PROGMEM = "OFF";

// SYSTEM INFO

const char RAM_TOTAL[] PROGMEM = "RAM %u bytes";
const char RAM_USE_FREE[] PROGMEM = "usage %u[%d%%], free %u[%d%%]";
const char SPIFFS_TOTAL[] PROGMEM = "SPIFFS %u bytes";
const char SPIFFS_USE_FREE[] PROGMEM = "usage %u[%d%%], free %u[%d%%]";


// ===== FUNCTIONS ===== //
const char F_ERROR_MAC[] PROGMEM = "ERROR: MAC address invalid";
const char F_COLOR_INVALID[] PROGMEM = "ERROR: Color code invalid";
const char F_ERROR_READING_FILE[] PROGMEM = "ERROR: reading file ";
const char F_LINE[] PROGMEM = "[%d] ";
const char F_ERROR_FILE[] PROGMEM = "ERROR: File doesn't exist ";
const char F_ERROR_OPEN[] PROGMEM = "ERROR couldn't open ";
const char F_ERROR_PARSING_JSON[] PROGMEM = "ERROR parsing JSON ";
const char F_ERROR_TO_BIG[] PROGMEM = "ERROR file too big ";
const char F_TMP[] PROGMEM = "/tmp";
const char F_COPY[] PROGMEM = "_copy";
const char F_ERROR_SAVING[] PROGMEM =
    "ERROR: saving file. Try 'format' and restart - ";

// ===== WIFI ===== //
const char W_STOPPED_AP[] PROGMEM = "Stopped Access Point";
const char W_AP_REQUEST[] PROGMEM = "[AP] request: ";
const char W_AP[] PROGMEM = "AP";
const char W_STATION[] PROGMEM = "Station";
const char W_MODE_OFF[] PROGMEM = "OFF";
const char W_MODE_AP[] PROGMEM = "AP";
const char W_MODE_ST[] PROGMEM = "STATION";
const char W_OK[] PROGMEM = " OK";
const char W_NOT_FOUND[] PROGMEM = " NOT FOUND";
const char W_BAD_ARGS[] PROGMEM = "BAD ARGS";
const char W_BAD_PATH[] PROGMEM = "BAD PATH";
const char W_FILE_NOT_FOUND[] PROGMEM = "ERROR 404 File Not Found";
const char W_STARTED_AP[] PROGMEM = "Started AP";

#endif // ifndef language_h
