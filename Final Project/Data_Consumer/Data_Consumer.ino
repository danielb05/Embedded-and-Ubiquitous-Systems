#include "SoftwareSerial.h"
#include "Wire.h"

SoftwareSerial xbeeSerial(2, 3);

#define SLAVE_ADDR 0x04
#define SLEEP 500

char received[20];

char msg[20];
int i=0;
char a;
char *distance, *temperature, *humidity;
char inputChar;
uint8_t i2cReceiver;
void setup()
{
    Serial.begin(9600);
    xbeeSerial.begin(9600);
    Wire.begin(SLAVE_ADDR);
    Wire.onRequest(response_data);
    Serial.println("Data Consumer");
}

void response_data(){

  char text[10];
  sprintf(text, "%s%s%s", distance, temperature, humidity);
  Serial.println(text);
  
  Wire.write(text,9);
  /*Serial.print("Sent to I2C: ");
  Serial.println(msg);
  delay(80);
  */
}

void loop()
{

    while (!xbeeSerial.available());
    a = xbeeSerial.read();
    received[i++] = a;
    if (a == 'y')
    {
        received[--i] = '\0';
        
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
}
