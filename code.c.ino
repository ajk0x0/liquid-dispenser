#include <MFRC522.h>
#include "db.h"

#define print(x) Serial.print(x)
#define SS_PIN 10
#define RST_PIN 9
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

void setup() { 
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  db::reset();
  if (!db::checkInit())
    db::initialize();
//  db::updateEntry(entry.address, entry.usage+1, Clock::getClock());
//  db::resetEntryAddress();
//  db::dumpData();
//  db::resetEntryAddress();
//  entry = db::nextEntry();
  db::dumpData();
}
 
void loop() {
  delay(500);
  Clock::updateClock();
  db::resetEntryAddress();
  unsigned long currentTime = Clock::getClock();
  if(!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;
  String uid = getUID(rfid.uid.uidByte, rfid.uid.size);
  print("has next: "); print(db::hasNextEntry());
  while(db::hasNextEntry()){
    db::Entry entry = db::nextEntry();
    char rfid[9];
    uid.toCharArray(rfid, 9);
    if (!strcmp(rfid, entry.uid)){
      if((currentTime - entry.lastUse) > 30000){
        print("time update");
        db::updateEntry(entry.address, 0, currentTime);
      }else if (entry.usage > 2){
        // reject code
        print("Card rejected");print("\n");
        return;
      }else{
        db::updateEntry(entry.address, entry.usage+1, currentTime);
      }
      // open outlet
    }else{
      print("rfid do not match : "); print(uid);
    }
  }
  db::dumpData();
}


/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
String getUID(byte *buffer, byte bufferSize) {
  String uid = "";
  uid = (char *) malloc(bufferSize*2);
  unsigned char firstNibble=0U;
  unsigned char secondNibble=0U;
  char firstHexChar=0;
  char secondHexChar=0;
  
  for (byte i = 0; i < bufferSize; i++) {
    firstNibble=(buffer[i]>>4);
    secondNibble=(buffer[i]&0x0F);
    if(firstNibble<10U)
      firstHexChar=(char)('0'+firstNibble);
    else{
     firstNibble-=10U;
     firstHexChar=(char)('A'+firstNibble);
    }
    if(secondNibble<10U)
     secondHexChar=(char)('0'+secondNibble);
    else{
     secondNibble-=10U;
     secondHexChar=(char)('A'+secondNibble);
    }
    uid += firstHexChar;
    uid += secondHexChar;
  }
  return uid;
}
