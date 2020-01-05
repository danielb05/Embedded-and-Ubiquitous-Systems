#include "SoftwareSerial.h"
#include "Wire.h"

SoftwareSerial xbeeSerial(2, 3);

#define SLAVE_ADDR 0x04
#define SLEEP 500

char received[255];

char msg[100];
int i=0;
char a;
char *distance, *temperature, *humidity;
char inputChar;

void setup()
{
    Serial.begin(9600);
    xbeeSerial.begin(9600);
    Serial.println("Data Consumer");
}

void loop()
{

    while (!xbeeSerial.available());
    a = xbeeSerial.read();
    received[i++] = a;
    if (a == 'y')
    {
        received[--i] = '\0';

        Wire.beginTransmission(SLAVE_ADDR);
        Wire.write(received);
        Wire.endTransmission();

        Serial.print("Sent to I2C: ");
        Serial.println(received);
        delay(80);

        Serial.print("Distance: ");
        distance = strtok(received, "x");
        Serial.print(distance);
        Serial.println("cm");        
        delay(80);

        Serial.print("Temperature: ");
        temperature = strtok(NULL, "x");
        Serial.print(temperature);
        Serial.println("ºC");
        delay(80);

        Serial.print("Humidity: ");
        humidity = strtok(NULL, "x");
        Serial.print(humidity);
        Serial.println("%");
        Serial.print("\n");
        delay(80);
        
        i = 0; 
    }

    while (!Serial.available())
    {
        inputChar = Serial.read();
        if (inputChar=='1'){
            Serial.print(distance);
            Serial.print('x');
            Serial.print(temperature);
            Serial.print('x');
            Serial.print(humidity);
            Serial.print('x');
        }
    }
    
}
