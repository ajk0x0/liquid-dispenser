#include <MFRC522.h>
#include "db.h"

#define print(x) Serial.print(x)
#define SS_PIN 10
#define RST_PIN 9
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class 

int BUZZER_PIN = 8;
int SUCCESS_BUZZ = 0;
int REJECT_BUZZ = 1;

void setup() { 
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  pinMode(BUZZER_PIN, OUTPUT);
  rfid.PCD_Init(); // Init MFRC522
  print(1>1);
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
  print("innn");
  String uid = getUID(rfid.uid.uidByte, rfid.uid.size);
  print("has next: "); print(uid);
  while(db::hasNextEntry()){
    db::Entry entry = db::nextEntry();
    char rfid[9];
    uid.toCharArray(rfid, 9);
    if (!strcmp(rfid, entry.uid)){
      if((currentTime - entry.lastUse) > 20000){
        print("time update");
        db::updateEntry(entry.address, 1, currentTime);
      }else if (entry.usage > 2){
        // reject code
        buzz(REJECT_BUZZ);
        print("Card rejected");print("\n");
        return;
      }else
        db::updateEntry(entry.address, entry.usage+1, currentTime);
      buzz(SUCCESS_BUZZ);
      delay(1000);
      // open outlet
    }else{
      print("rfid do not match : "); print(uid);
      buzz(REJECT_BUZZ);
    }
  }
//  db::dumpData();
}


void buzz(int op){
  if (op == SUCCESS_BUZZ){
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
  }
  else {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
  }
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
