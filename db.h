#include <EEPROM.h>
#include <string.h>
#include "data.h"

namespace db {
  bool isInitialized = false;
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
    clockTime = timePast;
    EEPROM.put(5, timePast);
  }

  void resetClock(){
    unsigned long timePast = 0;
    clockTime = 0;
    EEPROM.put(5, timePast);
  }

  bool checkInit(){
      char flag[5];
      EEPROM.get(0, flag);
      isInitialized = !strcmp(flag, "init");
      isInitialized ? (clockTime = getLoggedClock()) : (clockTime = 0);
      return isInitialized;
  };
  
  void initialize(){
    unsigned long timePast = 0;
    char usage = 0;
    clockTime = 0;
    EEPROM.put(0, "init");
    EEPROM.put(5, timePast);
    EEPROM.put(9, "830947A7");
    EEPROM.put(18,usage);
  }

  void reset(){
    EEPROM.put(0, "rese");
    resetClock();
  }
}
