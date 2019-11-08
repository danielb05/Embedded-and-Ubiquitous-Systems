/*
 * MASTER CONFIG
 */

#include <Adafruit_Sensor.h> 
#include <Adafruit_ADXL345_U.h>
#include "SoftwareSerial.h"
#include <lcd_lib.h>

SoftwareSerial lcdSerial(0, 1);
SoftwareSerial xbeeSerial(2, 3);

#define SLAVE_ADDR 0x04

char inputChar;
uint16_t cal16;
char cal8[255];
char *array[2];
char *data;

float X, Y, Z;  // Accelerometer values
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();
LCDLib lcdControl;
char *msg;
int aux;

void setup() {
  Serial.begin(9600);
  accelerometerSetUp();
  xbeeSerial.begin(9600);
  lcdControl.begin(&lcdSerial);
  printInitialMessages();
}

void loop() {
  
  while(!Serial.available()); //Wait for something

  inputChar=Serial.read();

  if (inputChar=='1' | inputChar=='2' | inputChar=='3'){
    //Serial.print("\nRequesting information");
    xbeeSerial.write(inputChar);
    int i = 0;
  if(inputChar == '1'){
    
      //Serial.println(" from the Ultrasonic sensor:");

     while(!xbeeSerial.available());
     while(xbeeSerial.available()){
      cal8[i]= xbeeSerial.read();
      delay(50);
      i++;
     }
     cal8[i]= '\0';
      
      printDistance();
  } else if(inputChar == '2'){
      //Serial.println(" from the DHT11 sensor:");
      
      while(!xbeeSerial.available());
     while(xbeeSerial.available()){
      cal8[i]= xbeeSerial.read();
      delay(50);
      i++;
     }
     cal8[i]= '\0';
     data = strtok(cal8, "x");
     array[0] = data;
     data = strtok(NULL, "x");
     array[1] = data;
      printTemperatureAndHumidity();
  } else if(inputChar == '3'){ 
      //Serial.println(" from the ADXL345 Accelerometer:");
      setAccelerometer();
      printAccelerometer();
  }
  }
}

void printInitialMessages(){
  /*Serial.println("MASTER");
  Serial.println("Type in:\n- 1 for Ultrasonic sensor (distance)\n- 2 for the DHT11 sensor (temperature and humidity)\n- 3 for the ADXL345 accelerometer (show each axis value)");*/
  lcdControl.eraseScreen();
  msg = "Press:";
  lcdControl.print(0, 63, msg);
  msg = "1 - for distance";
  lcdControl.print(0, 55, msg);
  msg = "2 - for tem & hum";
  lcdControl.print(0, 47, msg);
  msg = "3 - for accelerometer";
  lcdControl.print(0, 39, msg);
}

void printDistance(){
  /*Serial.println("Received: ");
  Serial.print("\nDistance: ");
  Serial.print(cal8);
  Serial.println("cm");*/
  lcdControl.eraseScreen();
  aux = (int)cal8;
  sprintf(msg, "Distance: %icm", aux);
  lcdControl.print(0, 63, msg);
}

void printTemperatureAndHumidity(){
    /*Serial.println("Received: ");
    Serial.print("Temperature: ");
    Serial.print(array[0]);
    Serial.print("\xC2\xB0");
    Serial.print("C and Humidity: ");
    Serial.print(array[1]);
    Serial.println("%");*/
    lcdControl.eraseScreen();
    aux = (int)array[0];
    sprintf(msg, "Temperature: %i deg C", aux);
    lcdControl.print(0, 63, msg);
    aux = (int)array[1];
    sprintf(msg, "Humidity: %i /100", aux);
    lcdControl.print(0, 47, msg);
}

void accelerometerSetUp(){
  if(!accel.begin())
   {
      Serial.println("No ADXL345 sensor detected.");
   }
}

void setAccelerometer(){
  sensors_event_t event; 
  accel.getEvent(&event);
  X = event.acceleration.x;
  Y = event.acceleration.y;
  Z = event.acceleration.z;
  delay(500);
}

void printAccelerometer(){  
/*  Serial.println("Acceleromter Data:");
  Serial.print("X = ");
  Serial.print(X);
  Serial.print("   Y = ");
  Serial.print(Y);
  Serial.print("   Z = ");
  Serial.println(Z);
*/
  lcdControl.eraseScreen();
  char x_temp[10], y_temp[10], z_temp[10];

  dtostrf(X, 6, 2, x_temp);
  sprintf(msg, "X: %s", x_temp);
  lcdControl.print(0, 63, msg);
  
  dtostrf(Y, 6, 2, y_temp);
  sprintf(msg, "Y: %s", y_temp);
  lcdControl.print(0, 47, msg);
  
  dtostrf(Z, 6, 2, z_temp);
  sprintf(msg, "Z: %s", z_temp);
  lcdControl.print(0, 31, msg);
}
