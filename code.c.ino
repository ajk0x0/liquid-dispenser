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
  if (!db::checkInit()){
    db::initialize();
  }
}
 
void loop() {
  db::updateClock();
  if(!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;
  String uid = getUID(rfid.uid.uidByte, rfid.uid.size);
  print(uid);
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
