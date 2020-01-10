/*
			University of Lleida
	Master Of Science in Computer Science
		Embedded And Ubiquitous Systems
				  Professor
		  Fernando Guirado Fernández
				Final Project
				  Students
			Antonio Expósito Solis
			Daniel Vieira Cordeiro
			Rafael Câmara Pereira
*/

// Include the libraries:
#include <Adafruit_Sensor.h>
#include "SoftwareSerial.h"
#include <DHT.h>

// Defines the PINS which will be used throught the code
#define SLAVE_ADDR 0x04
#define TRIG_PIN 12
#define ECHO_PIN 10
#define DHTPIN 7
#define DHTTYPE DHT11

// Variables
int duration, distance, temperature, humidity;

// Initialize DHT sensor for normal 16mhz Arduino:
DHT dht = DHT(DHTPIN, DHTTYPE);

// Defines the PINS which will be used for the serial communication with the XBee
SoftwareSerial xbeeSerial(2, 3);

// Initial SetUp
void setup() {
  
  //Serial Port begin
  Serial.begin(9600);
  xbeeSerial.begin(9600);
  
  //Define inputs and outputs
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Setup DHT11 sensor:
  dht.begin();

  // Used to facilitate the identification of the Serial Monitor 
  Serial.println("Data Producer");
}

// Main execution
void loop() {

  // Collecting data from the sensors
  setUltrasonic();
  setDHT11();
  
  // Sending the data
  sendFunc();
}

// Processes the sensors data and forwards it to the XBee Serial
void sendFunc(){

  // Printing on the Serial Monitor
  printDistance();
  printDHT11();

  // Variable that will store the data in the correct format to be sent
  char msg[20];

  // Aggregates the 3 values into a single char array, separated by 'x' and with a 'y' as end of line
  sprintf(msg, "%03dx%03dx%03dy", distance, temperature, humidity);

  // Sends the aggregated char array via the Xbee
  xbeeSerial.print(msg);

  // Waits 5 seconds
  delay(5000);
}

// SetUps the 
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

// Reads the signal from the sensor: a HIGH pulse whose duration is the time (in microseconds) from the sending of the ping to the reception of its echo off of an object.
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

// Gathers data from DHT11 sensor
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
