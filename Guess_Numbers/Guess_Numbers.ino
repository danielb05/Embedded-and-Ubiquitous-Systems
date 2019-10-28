const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data

int number;
int tip;

void setup()
{
  Serial.begin(9600);
  Serial.println("Guess a number from 1 to 1000");
  number = random(1000);
  Serial.println(number);
}

void loop()
{
  recvWithEndMarker();
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
      showNewData();
    }
  }
}

void showNewData()
{
  if (receivedChars[0] >= 48 && receivedChars[0] <= 57)
  {
    int input = -1;
    input = atoi(receivedChars);
    if (input == number)
    {
      Serial.println("You are correct!");
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

void randomHelp(){
  Serial.println("Help");
}
