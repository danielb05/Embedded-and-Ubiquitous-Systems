/*
 * SLAVE CONFIG
 */

// Include the libraries:
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "Wire.h"

// Defines
#define SLAVE_ADDR 0x04
#define TRIG_PIN 11
#define ECHO_PIN 12

// Variables
int duration, cm;
char accessVar;
uint16_t tmp16;
uint8_t tmp8;

void receiveFunc(){
  while(Wire.available()){
    accessVar=(char)Wire.read();
    Serial.print("Accessing: ");
    Serial.print((char)accessVar);
  }
}

void sendFunc(){
uint8_t val[2];

  Serial.print(" - Sending: ");
  switch (accessVar){
    case '1':
       val[0]=23;
       Serial.println(val[0]);
       Wire.write(val[0]);
      break;
    case '2':
       tmp16 = 2345;
       Serial.println(tmp16);
       val[0] = (tmp16>>8)&0xFF;
       val[1] = tmp16&0xFF;
       Wire.write(val,2);
      break;
  }
}

void setup() {
  
  //Serial Port begin
  Serial.begin(9600);
  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveFunc);
  Wire.onRequest(sendFunc);

  //Define inputs and outputs
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  
  get();
  
}

void getUltrasonic(){

  ultrasonicTrigger();
  readDuration();
  convertToCm();
}

// The sensor is triggered by a HIGH pulse of 10 or more microseconds.
// Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
void ultrasonicTrigger(){
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
}

// Read the signal from the sensor: a HIGH pulse whose duration is the time (in microseconds) from the sending of the ping to the reception of its echo off of an object.
void readDuration(){  
  pinMode(ECHO_PIN, INPUT);
  duration = pulseIn(ECHO_PIN, HIGH);
}

// Convert the time into a distance
void convertToCm(){
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
}
