// Include the libraries:
#include <Adafruit_Sensor.h>
#include "SoftwareSerial.h"
#include <DHT.h>

// Defines
#define TRIG_PIN 12
#define ECHO_PIN 10
#define DHTPIN 7
#define DHTTYPE DHT11

// Variables
int duration, distance, temperature, humidity;
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

  Serial.println("Data Producer");
  delay(75);
}

void loop() {

  char a;
  while (!xbeeSerial.available());
  a = xbeeSerial.read();
  delay(75);
  if(a == '1')
    getData();
}

void getData(){
  
  setUltrasonic();
  setDHT11();
  sendFunc();
}

void sendFunc(){
    
  printDistance();
  printDHT11();
  char msg[13];
  sprintf(msg, "%03dx%03dx%03dy", distance, temperature, humidity);
  delay(75);
  xbeeSerial.print(msg);
  delay(75);
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
  delay(75);
  Serial.print(distance);
  delay(75);
  Serial.println(" cm");
  delay(75);
}

void setDHT11(){

  // Read the humidity in %:
  humidity = dht.readHumidity();
  // Read the temperature as Celsius:
  temperature = dht.readTemperature();
}

// Prints the measured humidity and temperature into the Serial Display
void printDHT11() {
  Serial.print("Temperature: ");
  delay(75);
  Serial.print(temperature);
  delay(75);
  Serial.print("\xC2\xB0");
  delay(75);
  Serial.println("C");
  delay(75);
  Serial.print("Humidity: ");
  delay(75);
  Serial.print(humidity);
  delay(75);
  Serial.println("%");
  delay(75);
}
