#include <LiquidCrystal.h>
//The delay between each loop
const int TIME_INTERVAL = 10;

//initialize LCD object
int RSPin = 13;
int enablePin =12;
int d4 = 11;
int d5 = 10;
int d6 = 9;
int d7 = 8;
LiquidCrystal lcd(RSPin, enablePin, d4, d5, d6, d7);

int buttonPin = 7;
int lightPin = 0;

int lastButtonState = LOW;
int buttonState = LOW;

//the variables to control the game speed
int gameSpeed = 100;
int charge = 100;
int currentCharge = 0;

//tells which lane does the player's car is running on
int playerPosition = 0;

//to hold light data
int brightness;

//controls how fast the incoming vehicle initialized
int nextEnemy = 50;

//determine the state of the game
bool gameOver = false;
int score = 0;

//the array is to store the information of the position of the elements
//0 means nothing there, 1 means incoming vehicle, -1 means player
//the screen can display 32 characters in 2 rows
int gameBoard[32] = {0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16,2);
  Serial.println("game initialized");
  pinMode(buttonPin, INPUT);
  pinMode(lightPin, INPUT);
  lcd.clear();
  //set player position to the first item of the array
  gameBoard[0] = -1;
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  if(!gameOver)
  {
    brightness = analogRead(lightPin);
    //set the charge value based on the brightness
    charge = sqrt(brightness)*4;
    if(buttonState != lastButtonState)
    {
      if(buttonState == HIGH)
      //the button is pressed, switch player position
      {
        switchPlayerPosition();
      }
    }
    //draw the elements to the lcd based on the information of the elements
    draw();
    //move the elements
    moving();
    //create new incoming vehicle
    initializeEnemy();
    //incremented the score
    score += sqrt(brightness);
  }
  else
  //the game is over
  { 
    //output score to lcd
    lcd.setCursor(0, 0);
    lcd.print("GAME OVER!");
    lcd.setCursor(0, 1);
    lcd.print("SCORE: ");
    lcd.print(String(score));
    
    if(buttonState != lastButtonState)
    {
      if(buttonState == HIGH)
      {
         delay(500);
         //button is held for more than 0.5 second
         if(digitalRead(buttonPin) == HIGH)
         {
          //reinitialize the game to restart
          reset();
         }
      }
    }
  }
  lastButtonState = buttonState;
  delay(10);
  //current charge will be used to compared with gameSpeed and hence decide whether we move the incoming vehicle forward
  currentCharge += charge;
  
}

void switchPlayerPosition()
{
  if(gameBoard[0] == -1)
  {
    //the 16th element of the array will be displayed on the second row of the lcd
    gameBoard[16] = -1;
    gameBoard[0] = 0;
  }
  else
  {
    gameBoard[16] = 0;
    gameBoard[0] = -1;
  }
}

void draw()
{
  //iterate the list of elements
  for(int i = 0; i < 32; i++)
  {
    //the first argument of setCursor() determines the x coordinate
    //the second argument of setCursor() determines the y coordinate
    lcd.setCursor(i % 16, i / 16);
    if(gameBoard[i] > 0)
    //incoming vehicle
    {
      lcd.print('<');
    }
    else if(gameBoard[i] == 0)
    //nothing
    {
      lcd.print(' ');
    }
    else
    //player's car
    {
      lcd.print('>');
    }
  }
  
}

void moving()
{
  if(currentCharge > gameSpeed)
  //trigger move event
  {
    //iterate the list
    for(int i = 0; i < 32; i++)
    {
      if(gameBoard[i] == 1)
      //only incoming vehicle moves forward
      {
        //remove the vehicle from current position
        gameBoard[i] = 0;
        if(i >= 1)
        //not at the edge of lcd
        {
          if(gameBoard[i - 1] == -1)
          //player's car is on the next position, crash occurs
          {
            gameOver = true;
            lcd.clear();
          }
          else
          //player's car is not on the next position
          {
            if(i != 16)
            //gameBoard[16] is also the edge, we can move the vehicle forward if that's not the case
            {
              gameBoard[i - 1] = 1;
            }
          }
        }
      }
      
    }
    //reset currentCharge to 0
    currentCharge = 0;
  }
}
  
  


void initializeEnemy()
{
  //incremented the value of nextEnemy based on the brightness
  nextEnemy = nextEnemy + sqrt(brightness);
  if(nextEnemy >= gameSpeed)
  //trigger incoming vehicle initialization
  {
    //get a random number
    int num = rand() % 800;
    if(num < brightness)
    //create new incoming vehicle
    {
        //decide which lane to be the born location
        if(num < brightness / 2)
        {
          gameBoard[15] = 1;
        }
        else
        {
          gameBoard[31] = 1;
        }
        nextEnemy = 0;
    }
  }
}

void reset()
{
  gameOver = false;
  memset(gameBoard, 0, sizeof(gameBoard));
  score = 0;
  gameBoard[0] = -1;
}
