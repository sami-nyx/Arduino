
// Compatible with the Arduino IDE 1.0
// Library version:1.1
// download LiquidCrystal_I2C library first!!
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Key.h>
#include <Keypad.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

/*
   Maximun input chars
*/
#define MAX_INPUT 10

/*
   end of input char like 'Enter'
*/
#define TERMINATING_CHAR 'D'

/*
open and closed servo positions
*/
#define OPEN_POS  160
#define CLOSED_POS  110

int pos;

/*
   no. of keypad rows and coulomns
*/
const byte ROWS = 4;
const byte COLS = 4;

/*
   stores the number of chars that has been entered by the keypad
*/
int charCounter;

/*
   iterator for password
*/
char *iterator;

/*
   index for iterations
*/
int i;

/*
  holds last enterd key
*/
char customKey;

/*
  your pasword goes here
*/
char *password = "A2B6";

/*
   a char array to store user input
*/
char buffer[MAX_INPUT];

/*
   representive matrix for keypad
*/
char hexaKeys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

/*
   keypad pins definition
*/
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

Servo servo;

void setup()
{
   servo.attach(11);
   lcd.init(); // initialize the lcd
   lcd.init();
   // Print a message to the LCD.
   lcd.backlight();
}

void loop()
{
   if (getPassword())
   { // if pasword is valid then open the door (servo)
      lcd.clear();
      lcd.print("door is open");
      open();
   }
   else
   {
      lcd.clear();
      lcd.print("Wrong password");
      lcd.setCursor(0, 1);
      delay(500);
      for (i = 0; i < 3; i++) // lcd backlight blink
      {
         delay(100);
         lcd.noBacklight();
         delay(100);
         lcd.backlight();
      }
   }
}

boolean getPassword()
{

   lcd.clear();
   lcd.print("password:");
   lcd.setCursor(0, 1);
   /*
      reset counter for new input array
   */
   charCounter = 0;
   do
   {
      /*
         waite for next input char
      */
      while (!(customKey = customKeypad.getKey()))
         // do nothing
         ;
      /*
         append last char to buffer
      */
      buffer[charCounter] = customKey;
      // Serial.println(customKey);

      lcd.print(customKey);

      /*
         increment counter
      */
      charCounter++;

      /*
         check if MAX_INPUT is exeeded, or if terminating character showed
      */
   } while (charCounter < MAX_INPUT && customKey != TERMINATING_CHAR);

   /*
     set initialize iterator and index
   */
   iterator = password;
   i = 0;

   /*
      check if buffer and password are matching
   */
   while (*iterator != '\0')
   {
      if ((*iterator++) != buffer[i])
      {
         return false;
      }
      i++;
   }

   /*
      return false if buffer is longer than password
   */
   if (buffer[i] != TERMINATING_CHAR)
      return false;

   return true;
}

void open()
{
   for (pos = CLOSED_POS; pos < OPEN_POS; pos++)
   {
      servo.write(pos);
      delay(15);
   }
   delay(3000); // open the door for 3 seconds
   for (pos = OPEN_POS; pos > CLOSED_POS; pos--)
   {

      servo.write(pos);
      delay(15);
   }
}
