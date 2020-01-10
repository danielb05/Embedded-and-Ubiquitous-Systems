#include "SoftwareSerial.h"
#include "Wire.h"

// Defines the PINS which will be used for the serial communication with the XBee
SoftwareSerial xbeeSerial(2, 3);

// Defines the address of the Arduino that will be used by the I2C bus for communication
#define SLAVE_ADDR 0x04
#define SLEEP 80

// Variables
char received[20];
int i = 0;
char a;
char *distance, *temperature, *humidity;

// Initial SetUp
void setup()
{
    // Initianting the Serial communication
    Serial.begin(9600);

    // Initianting the XBee Serial communication
    xbeeSerial.begin(9600);

    // Joining the I2C Bus with the designated address
    Wire.begin(SLAVE_ADDR);
    
    // Registering the response_data() function to be called when a request arrives
    Wire.onRequest(response_data);

    // Used to facilitate the identification of the Serial Monitor 
    Serial.println("Data Consumer");
}

// Processes the sensors data and forwards it to the I2C bus
void response_data(){
  
  char text[10];
  // Aggregates the 3 values into a single char array
  sprintf(text, "%s%s%s", distance, temperature, humidity);
  
  Serial.println(text);

  // Sends the aggregated char array via the I2C bus
  Wire.write(text,9);
}

// Main execution
void loop()
{
    // Awaiting to receive some data
    while (!xbeeSerial.available());

    // Data received via the XBee Serial
    a = xbeeSerial.read();

    // Storing the received byte on an array
    received[i++] = a;

    // Checks if a 'y' (which in this case means end of transmited data) is received
    if (a == 'y')
    {
        // Replaces the 'y' by an end of line to allow the strtok() to work properly
        received[--i] = '\0';
        
        Serial.print("Distance: ");

        // Stores the values up to the first 'x', correspondent to the distance on the appropriate variable
        distance = strtok(received, "x");
        Serial.print(distance);
        Serial.println("cm");        
        delay(SLEEP);

        // Stores the values up to the second 'x', correspondent to the temperature on the appropriate variable
        Serial.print("Temperature: ");
        temperature = strtok(NULL, "x");
        Serial.print(temperature);
        Serial.println("ºC");
        delay(SLEEP);

        // Stores the values up to the third 'x', correspondent to the humidity on the appropriate variable
        Serial.print("Humidity: ");
        humidity = strtok(NULL, "x");
        Serial.print(humidity);
        Serial.println("%");
        Serial.print("\n");
        delay(SLEEP);

        // Resets the variable which counts the size of the received data
        i = 0; 
    }    
}
