boolean blinking;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  blinking = false;
}

// the loop function runs over and over again forever
void loop() {
  char carac;

  if(Serial.available()){
    carac = Serial.read();
  }

  switch(carac){
    case 'B':
      blinking = true;
      break;
    case 'S':
      blinking = false;
      break;
    case 'A':
      if(blinking)
        Serial.println("LED is Blinking");
      else
        Serial.println("LED is NOT Blinking");
      break;
  }
  carac = 'q';
  if(blinking){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                       // wait for a second
  }

}
