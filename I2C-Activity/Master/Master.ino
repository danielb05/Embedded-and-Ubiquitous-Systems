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
  Serial.println("MASTER");
  Serial.println("Type in:\n- 1 for Ultrasonic sensor (istance)\n- 2 for the DHT11 sensor (temperature and humidity)");
}

void loop() {
  
  while(!Serial.available()); //Wait for something

  inputChar=Serial.read();

  if (inputChar=='1' | inputChar=='2'){
    Serial.print("\nRequesting information");
    
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(inputChar);
    Wire.endTransmission();
    
    if (inputChar=='1') {
      Serial.print(" from the Ultrasonic sensor:");
      Wire.requestFrom(SLAVE_ADDR,1);

      cal8= Wire.read();

      Serial.print("\nReceived: ");
      Serial.print("\nDistance: ");
      Serial.print(cal8);
      Serial.println("cm");
    } 
    else {
        Serial.println(" from the DHT11 sensor:");
        Wire.requestFrom(SLAVE_ADDR,2);
        while(Wire.available()!=2);
        cal16=(Wire.read()<<8);
        cal16=cal16|Wire.read();
        uint8_t array[2];
        array[0]=cal16 & 0xff;
        array[1]=(cal16 >> 8);

        Serial.println("Received: ");
        Serial.print("Temperature: ");
        Serial.print(array[0]);
        Serial.print("\xC2\xB0");
        Serial.print("C and Humidity: ");
        Serial.print(array[1]);
        Serial.println("%");
    }
  }
}
