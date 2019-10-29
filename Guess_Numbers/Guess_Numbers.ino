const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data

int number;
int attempts;
int numberClues;

bool gameOngoing = false;

void setup()
{
  randomSeed(analogRead(0));
  Serial.begin(9600);
}

void loop()
{
  if (!gameOngoing)
  {
    setUpGame();
  }
  recvWithEndMarker();
}

void setUpGame()
{
  Serial.println("Guess a number from 1 to 1000");
  Serial.println("\nPress h for a clue. \n");
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
      Serial.println("\nYou are correct!");
      Serial.print("\nThe number was: ");
      Serial.println(number);
      Serial.print("\nNumber of attempts needed: ");
      Serial.println(attempts);
      Serial.print("Number of clues needed: ");
      Serial.println(numberClues);
      Serial.println("\n\nYOU WON!!!\n\n");
      gameOngoing = false;
    }
    else if (input > number)
    {
      Serial.println("Try a smaller number.");
    }
    else
    {
      Serial.println("Try a greater number.");
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
    if (number % 2 == 0)
    {
      Serial.println("It is an EVEN number.");
    }
    else
    {
      Serial.println("It is an ODD number.");
    }
    break;

  // number of digits
  case 1:
    if (number > 999)
    {
      Serial.println("The number has 4 digits - xxxx");
    }
    else if (number > 99 && number < 999)
    {
      Serial.println("The number has 3 digits - xxx");
    }
    else if (number < 99 && number > 9)
    {
      Serial.println("The number has 2 digits - xx");
    }
    else
    {
      Serial.println("The number has 1 digit - x");
    }
    break;

  // Last digit
  case 2:

    Serial.print("The last digit of the number is: ");
    Serial.println(number % 10);
    break;

    // First digit
  case 3:

    int num = number;
    while (num >= 10)
    {
      num = num / 10;
    }

    Serial.print("The first digit of the number is: ");
    Serial.println(num);
    break;
    
  }
}
