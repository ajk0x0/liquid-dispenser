#include <EEPROM.h>
#include <string.h>
#include "data.h"

namespace Clock {
  unsigned long clockTime ;
  
  unsigned long getClock(){
    unsigned long timePast = clockTime + millis();
    return timePast;
  }

  unsigned long getLoggedClock(){
    unsigned long timePast;
    EEPROM.get(5, timePast);
    return timePast;
  }
  
  void updateClock(){
    unsigned long timePast = clockTime + millis();
    EEPROM.put(5, timePast);
  }

  void resetClock(){
    unsigned long timePast = 0;
    clockTime = 0;
    EEPROM.put(5, timePast);
  }
}


namespace db {
  bool isInitialized = false;
  unsigned int address = 11;
  unsigned int entries;
  unsigned currentEntry = 0;

  struct Entry {
    char uid[9];
    char usage;
    unsigned int address;
    unsigned long lastUse;
  };
  
  bool checkInit(){
      char flag[5];
      EEPROM.get(0, flag);
      isInitialized = !strcmp(flag, "init");
      isInitialized ? (Clock::clockTime = Clock::getLoggedClock()) : (Clock::clockTime = 0);
      EEPROM.get(9, entries);
      return isInitialized;
  };
  void initialize(){
    Serial.print("Initializing");Serial.print("\n");
    unsigned long timePast = 0;
    unsigned long lastUse = 0;
    entries = 1;
    char usage = 0;
    Clock::clockTime = 0;
    EEPROM.put(0, "init");
    EEPROM.put(5, timePast);
    EEPROM.put(9, entries);
    EEPROM.put(11, "830947A7");
    EEPROM.put(20,usage);
    EEPROM.put(21, lastUse);
  }

  bool hasNextEntry() {
    return currentEntry < entries;
  }
  
  Entry nextEntry(){
    char uid[9];
    char usage;
    unsigned long lastUse;
    EEPROM.get(11, uid);
    EEPROM.get(20, usage);
    EEPROM.get(21, lastUse);
    Entry line;
    strcpy(line.uid, uid);
    line.usage = usage;
    line.address = address;
    line.lastUse = lastUse;
    address += 10;
    currentEntry++;
    return line;
  }

  void updateEntry(unsigned int address, char usage, unsigned long currentTime){
    Serial.print("\n");Serial.print((int) usage);Serial.print("\t updated \n");
    EEPROM.put(address+9, usage);
    EEPROM.put(address+10, currentTime);
  }
  
  void dumpData(){
    char init[5];
    unsigned long timePast;
    unsigned int entries;
    char uid[9];
    char usage;
    unsigned long lastUse;
    EEPROM.get(0, init);
    EEPROM.get(5, timePast);
    EEPROM.get(9, entries);
    EEPROM.get(11, uid);
    EEPROM.get(20, usage);
    EEPROM.get(21, lastUse);
    Serial.print(init);Serial.print("\t");
    Serial.print(timePast);Serial.print("\t");
    Serial.print(entries);Serial.print("\t");
    Serial.print(uid);Serial.print("\t");
    Serial.print((int) usage);Serial.print("\t");
    Serial.print(lastUse);Serial.print("\n");
    
  }

  void resetEntryAddress(){
    address = 11;
    currentEntry = 0;
  }
  
  void reset(){
    EEPROM.put(0, "rese");
    Clock::resetClock();
  }
}
