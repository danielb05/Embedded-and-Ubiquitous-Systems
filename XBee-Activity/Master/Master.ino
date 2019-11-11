/*
 * MASTER CONFIG
 */

#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include "SoftwareSerial.h"
#include <lcd_lib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

SoftwareSerial lcdSerial(2, 3);

#define SLAVE_ADDR 0x04
#define SLEEP 500

char received[255];
char *distance, *temperature, *humidity;

float X, Y, Z; // Accelerometer values
int xi, xd, yi, yd, zi, zd;

char msg[100];
int i=0;
char a;

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();
LCDLib lcdControl;


void setup()
{
    Serial.begin(9600);
    accelerometerSetUp();
    lcdControl.begin(&lcdSerial);   
    lcdControl.eraseScreen(); 
    initialValues();
}

void loop()
{

    while (!Serial.available());
    a = Serial.read();
    received[i++] = a;
    if (a == 'y')
    {
        lcdControl.eraseScreen();
        received[--i] = '\0';
        distance = strtok(received, "x");
        temperature = strtok(NULL, "x");
        humidity = strtok(NULL, "x");

        printDistance();
        printTemperatureAndHumidity();
        i = 0; 
        getAccelerometer(); 
        printAccelerometer();
    }
}

void initialValues(){
  lcdControl.print(0, 63, "Distance: ");
  lcdControl.print(0, 55, "Temperature: ");
  lcdControl.print(0, 47, "Humidity: ");
  lcdControl.print(0, 39, "X: ");
  lcdControl.print(0, 31, "Y: ");
  lcdControl.print(0, 23, "Z: ");
}

void printDistance()
{
    sprintf(msg, "Distance: %scm", distance);
    lcdControl.print(0, 63, msg);
}

void printTemperatureAndHumidity()
{
    sprintf(msg, "Temperature: %s deg C", temperature);
    lcdControl.print(0, 55, msg);

    sprintf(msg, "Humidity: %s percent", humidity);
    lcdControl.print(0, 47, msg);
}

void accelerometerSetUp()
{
    if (!accel.begin())
    {
        Serial.println("No ADXL345 sensor detected.");
    }
}

void getAccelerometer()
{
    sensors_event_t event;
    accel.getEvent(&event);
    X = event.acceleration.x;
    Y = event.acceleration.y;
    Z = event.acceleration.z;
    delay(SLEEP);

    castToInts();
}

void printAccelerometer()
{
    sprintf(msg, "X: %i.%i ", xi, xd);
    lcdControl.print(0, 39, msg);

    sprintf(msg, "Y: %i.%i ", yi, yd);
    lcdControl.print(0, 31, msg);

    sprintf(msg, "Z: %i.%i ", zi, zd);
    lcdControl.print(0, 23, msg);
}

void castToInts()
{

    double aux1, aux2;

    aux1 = modf(X, &aux2);
    xd = (int)(aux1 * 100);
    xi = (int)aux2;

    if (xd < 0)
        xd = xd * -1;

    aux1 = modf(Y, &aux2);

    yd = (int)(aux1 * 100);
    yi = (int)aux2;

    if (yd < 0)
        yd = yd * -1;

    aux1 = modf(Z, &aux2);
    zd = (int)(aux1 * 100);
    zi = (int)aux2;

    if (zd < 0)
        zd = zd * -1;
}
