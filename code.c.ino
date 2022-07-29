#include <MFRC522.h>
#include <string.h>
#include "waterflow.h"
#include "wifi.h"

#define print(x) Serial.println(x)
#define SS_PIN D4
#define RST_PIN D3
#define BUZZER_PIN D8
#define SUCCESS_BUZZ 0
#define REJECT_BUZZ 1
#define RELAY_PIN D9

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class 

void setup() { 
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  pinMode(BUZZER_PIN, OUTPUT);
  rfid.PCD_Init(); // Init MFRC522
  waterflow::initialize();
  wifi::initialize();
}

void loop() {
  if(!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;
  waterflow::reset();
  String uid = getUID(rfid.uid.uidByte, rfid.uid.size);
  String data = wifi::getDetails(uid);
  if (!strcmp(data, "OK")){
    buzz(SUCCESS_BUZZ);
    digitalWrite(RELAY_PIN, HIGH);
    while(waterflow::dispensedMl() < 250) delay(10);
    digitalWrite(RELAY_PIN, LOW);
  }else buzz(REJECT_BUZZ);
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
  unsigned char firstNibble=pre1U;
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
