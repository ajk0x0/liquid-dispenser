namespace waterflow {
  byte sensorInterrupt = 0;
  byte sensorPin       = D2;
  float calibrationFactor = 4.5;
  volatile byte pulseCount;  
  float flowRate;
  unsigned int flowMilliLitres;
  unsigned long totalMilliLitres;
  unsigned long oldTime;

  void ICACHE_RAM_ATTR pulseCounter(){
    pulseCount++;
  }
  
  void initialize(){
    pinMode(sensorPin, INPUT);
    digitalWrite(sensorPin, HIGH);
  
    pulseCount = 0;
    flowRate = 0.0;
    flowMilliLitres = 0;
    totalMilliLitres = 0;
    oldTime = 0;
    
    digitalWrite(sensorPin, HIGH);
    attachInterrupt(digitalPinToInterrupt(sensorPin), waterflow::pulseCounter, RISING);
  }
  
  unsigned long dispensedMl(){
      delay(50);
      detachInterrupt(sensorInterrupt);
      flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
      oldTime = millis();
      flowMilliLitres = (flowRate / 60) * 1000;
      totalMilliLitres += flowMilliLitres;
      pulseCount = 0;
      attachInterrupt(digitalPinToInterrupt(sensorPin), waterflow::pulseCounter, RISING);
      return totalMilliLitres;
  }

  void reset(){
    pulseCount        = 0;
    flowRate          = 0.0;
    flowMilliLitres   = 0;
    totalMilliLitres  = 0;
    oldTime           = 0;
  }
}
