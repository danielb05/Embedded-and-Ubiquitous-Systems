#include "SoftwareSerial.h"
#include "Wire.h"

SoftwareSerial xbeeSerial(2, 3);

#define SLAVE_ADDR 0x04
#define SLEEP 500

char received[12];

char msg[10];
int i = 0, requesting = 0;
char a = 'y';
char *distance, *temperature, *humidity;
char inputChar;
uint8_t i2cReceiver;

void setup()
{
    Serial.begin(9600);
    xbeeSerial.begin(9600);
    delay(75);
    distance = 0;
    temperature = 0;
    humidity = 0;    
    Wire.begin(SLAVE_ADDR);
    delay(75);
    Wire.onRequest(response_data);
    delay(75);
    Serial.println("Data Consumer");
    delay(75);
}

void loop(void){

  requestProd();    
}

void response_data(){

  char text[10];
  sprintf(text, "%s%s%s", distance, temperature, humidity);
  Serial.println(text);
  
  Wire.write(text,9);
}

void requestProd()
{

  if (requesting == 0)
  {
    xbeeSerial.print('1');
    delay(75);
    requesting = 1;
  }
      
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
      requesting = 0;

      delay(3000);
  }    
}
