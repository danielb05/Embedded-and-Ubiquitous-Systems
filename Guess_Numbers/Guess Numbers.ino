#include <stdio.h>
#include <stdlib.h>

int number;

void setup() {
  
  randomSeed(analogRead(0));
  number = random(1000);
  Serial.begin(9600);
  Serial.println("Guess a number from 1 to 1000");
  Serial.println(number);
  
}

char rx_byte = 0;
String rx_str = "";
boolean not_number = false;

void loop() {

  int input = -1;
  
  if (Serial.available() > 0) {
    
    rx_byte = Serial.read();
    
    if ((rx_byte >= '0') && (rx_byte <= '9')) {
      rx_str += rx_byte;
    }
    else if (rx_byte == '\n') {
      
      if (rx_byte == '\n') {      
        input = rx_str.toInt() * 2;
        not_number = false;
      }
      
      not_number = false;
      rx_str = "";
      
    }  
    else {
      not_number = true;
    }
  }

  if(not_number){
    //random tip
  }

  else {
    if(input == number){
        Serial.println("You are correct!");
    }
    else if(input > number){
        Serial.println("Try a smaller number.");
    }
    else {
        Serial.println("Try a greater number.");
    }
  }
}
