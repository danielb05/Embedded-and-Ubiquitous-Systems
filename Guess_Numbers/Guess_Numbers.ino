#include <lcd_lib.h>

LCDLib lcdControl;
SoftwareSerial lcdSerial(2, 3);
const byte numChars = 32;
char *msg;
char receivedChars[numChars]; // an array to store the received data

int number;
int attempts;
int numberClues;

bool gameOngoing = false;

void setup()
{
  randomSeed(analogRead(0));
  lcdControl.begin(&lcdSerial);
  Serial.begin(9600);
}

void loop()
{
  if (!gameOngoing)
  {
    startGameScreen();
  }else{
    recvWithEndMarker();
  }
}

void startGameScreen()
{

  int i, j;
  char rc;
  
  lcdControl.rectangle(0, 0, 127, 63, 1);
  delay(90);
  lcdControl.circle(63, 32, 30, 1);
  delay(90);
  lcdControl.line(0, 0, 127, 63, 1);
  delay(90);
  lcdControl.line(0, 63, 127, 0, 1);

  delay(3000);
  for(i = 63; i >= 0; i--){
    for(j = 0; j < 128; j++){
      lcdControl.sPixel(j,i);
    }
  }
  delay(90);
  lcdControl.eraseBox(63);
  setUpGame();
      
//gameOngoing = true;
}

void setUpGame()
{
  msg = "Guess a number from 1 to 1000.";
  lcdControl.print(0, 63, msg);
  msg = "Press h for a clue.";
  lcdControl.print(0, 47, msg);
  number = random(1001);
  //Serial.println(number);
  gameOngoing = true;
  attempts = 0;
  numberClues = 0;
}

void recvWithEndMarker()
{
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  while (Serial.available() > 0)
  {
    rc = Serial.read();

    if (rc != endMarker)
    {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars)
      {
        ndx = numChars - 1;
      }
    }
    else
    {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      avaluateInput();
    }
  }
}

void avaluateInput()
{
  if (receivedChars[0] >= 48 && receivedChars[0] <= 57)
  {
    attempts++;
    int input = -1;
    input = atoi(receivedChars);
    if (input == number)
    {
      lcdControl.eraseScreen();
      msg = "You are correct!";
      lcdControl.print(0, 63, msg);
      msg = "The number was:";
      lcdControl.print(0, 55, msg);
      sprintf(msg, "%d", number);
      lcdControl.print(0, 47, msg);
      msg = "Attempts:";
      lcdControl.print(0, 39, msg);
      sprintf(msg, "%d", attempts);
      lcdControl.print(0, 31, msg);
      msg = "Clues:";
      lcdControl.print(0, 23, msg);
      sprintf(msg, "%d", numberClues);
      lcdControl.print(0, 15, msg);
      msg = "YOU WON!!!";
      lcdControl.print(0, 7, msg);
      gameOngoing = true;
    }
    else
    {
      lcdControl.eraseBox(39);
      delay(90);
      
      if (input > number)
      {
        msg = "Try a smaller number.";
      }
      else
      {
        msg = "Try a greater number.";
      }
      
      lcdControl.print(0, 39, msg);
    }
  }
  else if (receivedChars[0] == 'h' || receivedChars[0] == 'H')
  {
    randomHelp();
  }
}

void randomHelp()
{

  numberClues++;
  int help = random(4);
  //Serial.println(help);
  switch (help)
  {

  case 0: // Even or Odd
    lcdControl.eraseBox(39);
    delay(90);
    if (number % 2 == 0)
    {
      msg = "It is an EVEN number.";
    }
    else
    {
      msg = "It is an ODD number.";
    }
    lcdControl.print(0, 39, msg);
    break;

  // number of digits
  case 1:
    lcdControl.eraseBox(39);
    delay(90);
    if (number > 999)
    {
      msg = "It has 4 digits.";
    }
    else if (number > 99 && number < 1000)
    {
      msg = "It has 3 digits.";
    }
    else if (number < 100 && number > 9)
    {
      msg = "It has 2 digits.";
    }
    else
    {
      msg = "It has 1 digit.";
    }
    lcdControl.print(0, 39, msg);
    break;

  // Last digit
  case 2:

    lcdControl.eraseBox(39);
    delay(90);  
    msg = "The last digit is: ";
    lcdControl.print(0, 39, msg);
    delay(90);
    sprintf(msg, "%d", number % 10);
    lcdControl.print(0, 31, msg);
    break;

    // First digit
  case 3:

    int num = number;
    while (num >= 10)
    {
      num = num / 10;
    }

    lcdControl.eraseBox(39);
    delay(90);  
    msg = "The first digit is: ";
    lcdControl.print(0, 39, msg);
    delay(90);
    sprintf(msg, "%d", num);
    lcdControl.print(0, 31, msg);
    break;
  }
}
