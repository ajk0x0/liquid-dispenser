#include <EEPROM.h>
#include <string.h>
#include "utils.h"

namespace db {
  void writeLine(int address, String id ){
    EEPROM.put(address, id);
  }
  void readLine(int address);
  void updateLine(int address);
  bool isInitialized(){
      char letter[4];
      EEPROM.get(0, letter);
      return !strcmp(letter, "init");
  };
  void initialize(){
    EEPROM.put(0, "init");
  }
}
