/*
 * MASTER CONFIG
 */

#include "Wire.h"

#define SLAVE_ADDR 0x04

char inputChar;
uint16_t cal16;
uint8_t cal8;


void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  
  while(!Serial.available()); //Wait for something

  inputChar=Serial.read();

  if (inputChar=='1' | inputChar=='2'){
    Serial.print("Request: ");
    Serial.print(inputChar);
    Serial.print(" - Receiving: ");
    
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(inputChar);
    Wire.endTransmission();
    
    if (inputChar=='1'){
      Wire.requestFrom(SLAVE_ADDR,1);
      cal8= Wire.read();
      Serial.println(cal8);
    } else {
        Wire.requestFrom(SLAVE_ADDR,2);
        while(Wire.available()!=2);
        cal16=(Wire.read()<<8);
        cal16=cal16|Wire.read();
        Serial.println(cal16);
    }
  }
}
