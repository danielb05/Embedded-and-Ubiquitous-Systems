/*
 * SLAVE CONFIG
 */

// Include the libraries:
#include <Adafruit_Sensor.h>
#include "SoftwareSerial.h"
#include <DHT.h>

// Defines
#define SLAVE_ADDR 0x04
#define TRIG_PIN 12
#define ECHO_PIN 11
#define DHTPIN 7
#define DHTTYPE DHT11

// Variables
int duration, distance;
float temperature, humidity;
DHT dht = DHT(DHTPIN, DHTTYPE); // Initialize DHT sensor for normal 16mhz Arduino:

SoftwareSerial xbeeSerial(2, 3);

void setup() {
  
  //Serial Port begin
  Serial.begin(9600);
  xbeeSerial.begin(9600);
  
  //Define inputs and outputs
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Setup DHT11 sensor:
  dht.begin();

  Serial.println("SLAVE");
}

void loop() {
  setUltrasonic();
  setDHT11();
  sendFunc();
}

void sendFunc(){
    
  printDistance();
  printDHT11();
 
  xbeeSerial.print(distance);
  xbeeSerial.print('x');
  xbeeSerial.print(temperature);
  xbeeSerial.print('x');
  xbeeSerial.print(humidity);
  delay(5000);
}

void setUltrasonic(){
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
// Distance = (traveltime/2) x speed of sound
// The speed of sound is: 343m/s = 0.0343 cm/uS = 1/29.1 cm/uS
void convertToCm(){
  distance = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
}

// Prints the measured distance in cm into the Serial Display
void printDistance() {
  Serial.print("\nDistance: ");
  Serial.print(distance);
  Serial.println(" cm");
}

void setDHT11(){
  // Wait a few seconds between measurements:
  delay(2000);
  // Read the humidity in %:
  humidity = dht.readHumidity();
  // Read the temperature as Celsius:
  temperature = dht.readTemperature();
}

// Prints the measured humidity and temperature into the Serial Display
void printDHT11() {
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("\xC2\xB0");
  Serial.println("C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

}
