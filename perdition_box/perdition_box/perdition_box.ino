#include <Wire.h>
#include "LedControl.h" //  need the library
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h" //  need the library

// ***** MEMORY *****
int button[] = {768, 510, 164, 91}; //The four button input pins
int ledpin[] = {8, 9, 12, 13};  // LED pins
int buttonstate = 0;  // button state checker
int sequenceLength = 2;
int randomArray[4];
int inputArray[4];
int btn_down_duration = 600;
boolean ingame = true;
int seq = 0;
// ***** MEMORY *****

// ***** LED MATRIX *****
int DIN_PIN = 6;      // data in pin
int CS_PIN = 7;       // load (CS) pin
int CLK_PIN = 5;      // clock pin
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 4); //
// ***** LED MATRIX *****


SoftwareSerial mySoftwareSerial(2, 3); // RX, TX

DFRobotDFPlayerMini myDFPlayer;


const unsigned char heart[] =
{
  01100110,
  11111111,
  11111111,
  11111111,
  01111110,
  00111100,
  00011000,
  00000000
};

const unsigned char error[] =
{
  B00000000,
  B01000010,
  B00100100,
  B00011000,
  B00100100,
  B01000010,
  B00000000,
  B00000000
};

const unsigned char smile[] =
{
  B00000000,
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10100101,
  B01011010,
  B00111100,
};

// ***** LED MATRIX *****

void setup()
{
 Serial.begin(115200);
/*
   mySoftwareSerial.begin(9600);
  

  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.

    Serial.println(F("Unable to begin:"));

    Serial.println(F("1.Please recheck the connection!"));

    Serial.println(F("2.Please insert the SD card!"));

    while(true);

  }


  Serial.println("DFPlayer Mini online.");

  myDFPlayer.volume(5);  //Set volume value. From 0 to 30

*/


  

  // init Leds
  for (int x = 0; x < 4; x++) // LED pins are outputs
  {
    pinMode(ledpin[x], OUTPUT);
  }

  randomSeed(analogRead(0)); //Added to generate "more randomness" with the randomArray for the output function

  // ***** LED MATRIX *****
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 1); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  lc.shutdown(1, false); // turn off power saving, enables display
  lc.setIntensity(1, 1); // sets brightness (0~15 possible values)
  lc.clearDisplay(1);// clear screen
  lc.shutdown(2, false); // turn off power saving, enables display
  lc.setIntensity(2, 1); // sets brightness (0~15 possible values)
  lc.clearDisplay(2);// clear screen
  lc.shutdown(3, false); // turn off power saving, enables display
  lc.setIntensity(3, 1); // sets brightness (0~15 possible values)
  lc.clearDisplay(3);// clear screen
  // ***** LED MATRIX *****
}

void loop()
{
/*
   myDFPlayer.play(1);  //Play the first mp3

delay(4000);
*/

  // ***** MEMORY *****
  // Generate random sequence
  ingame = true;
  Serial.println("random array:");
  for (int y = 0; y < sequenceLength; y++)
  {
    int rnd = random(4);
    randomArray[y] = rnd;
    Serial.print(randomArray[y]);
    Serial.println("");
    digitalWrite(ledpin[rnd], HIGH);
    delay(500);
    digitalWrite(ledpin[rnd], LOW);
    delay(200);
  }

  memory_input();
  // ***** MEMORY *****
}

void memory_input() { //Function for allowing user input and checking input against the generated array

  while (ingame) {

      int btnVal= analogRead(A0);

      if (btnVal > 10)
      { //Checking for button push

      int btnNum;

      if (btnVal > 750 && btnVal < 770) {
        btnNum = 0;
      } else if (btnVal > 500 && btnVal < 520) {
        btnNum = 1;
      } else if (btnVal > 160 && btnVal < 170) {
        btnNum = 2;
       }
     else if (btnVal > 80 && btnVal < 100) {
        btnNum = 3;
       } else {
         continue;
         }


        Serial.print("button pressed: ");
        Serial.println(btnNum + 1);
        Serial.print("seq: ");
        Serial.println(seq);

        digitalWrite(ledpin[btnNum], HIGH);
        delay(btn_down_duration);
        digitalWrite(ledpin[btnNum], LOW);
        inputArray[seq] = btnNum;

        if (inputArray[seq] != randomArray[seq]) { //Checks value input by user and checks it against
          lose();                             //the value in the same spot on the generated array
        } else {
          ++seq;
          if (seq == sequenceLength) {
            win();
          }
        }

      }
  }
}

void win() { //Function used if the player fails to match the sequence
  resetVars();

  draw4Smiles();

  for (int x = 0; x < 8; x++) {
    for (int led = 0; led < 4; led++) {
      digitalWrite(ledpin[led], HIGH);

      delay(50);
      // stop for the next note:
      //speakerpin.stop();
      digitalWrite(ledpin[led], LOW);

    }
    for (int led = 4; led > 0; led--) {
      digitalWrite(ledpin[led], HIGH);

      delay(50);
      // stop for the next note:
      //speakerpin.stop();
      digitalWrite(ledpin[led], LOW);

    }
    x++;
  }

  delay(2000);

  clearMatrix();
}

void lose() { //Function used if the player fails to match the sequence
  resetVars();

  for (int y = 0; y < 3; y++)
  { //Flashes lights for failure
    drawError();
    /*
        digitalWrite(ledpin[0], HIGH);
        digitalWrite(ledpin[1], HIGH);
        digitalWrite(ledpin[2], HIGH);
        digitalWrite(ledpin[3], HIGH);
    */
    delay(300);
    clearMatrix();
    /*
      digitalWrite(ledpin[0], LOW);
      digitalWrite(ledpin[1], LOW);
      digitalWrite(ledpin[2], LOW);
      digitalWrite(ledpin[3], LOW);
    */
    delay(100);
  }

  delay(2000);
}

void resetVars() {
  ingame = false;
  seq = 0;
  memset(inputArray, 0, sizeof(inputArray));
  memset(randomArray, 0, sizeof(randomArray));
}

// ***** LED MATRIX *****
void putByte(byte data)
{
  byte i = 8;
  byte mask;
  while (i > 0)
  {
    mask = 0x01 << (i - 1);        // get bitmask
    digitalWrite( CLK_PIN, LOW);   // tick
    if (data & mask)               // choose bit
      digitalWrite(DIN_PIN, HIGH); // send 1
    else
      digitalWrite(DIN_PIN, LOW);  // send 0
    digitalWrite(CLK_PIN, HIGH);   // tock
    --i;                           // move to lesser bit
  }
}

void drawError() {

  for (int dev = 0; dev < 4; dev++)
  {
    lc.setLed(dev, 1, 1, true); // turns on LED at col, row
    lc.setLed(dev, 2, 2, true); // turns on LED at col, row
    lc.setLed(dev, 3, 3, true); // turns on LED at col, row
    lc.setLed(dev, 4, 4, true); // turns on LED at col, row
    lc.setLed(dev, 5, 5, true); // turns on LED at col, row
    lc.setLed(dev, 6, 6, true); // turns on LED at col, row

    lc.setLed(dev, 1, 6, true); // turns on LED at col, row
    lc.setLed(dev, 2, 5, true); // turns on LED at col, row
    lc.setLed(dev, 3, 4, true); // turns on LED at col, row
    lc.setLed(dev, 4, 3, true); // turns on LED at col, row
    lc.setLed(dev, 5, 2, true); // turns on LED at col, row
    lc.setLed(dev, 6, 1, true); // turns on LED at col, row
  }
}

void draw4Smiles() {
  for (int dev = 0; dev < 4; dev++)
  {
    drawSmile(dev);
  }
}

void drawSmile(int dev) {
    lc.setLed(dev, 0, 2, true); // turns on LED at col, row
    lc.setLed(dev, 0, 3, true); // turns on LED at col, row
    lc.setLed(dev, 0, 4, true); // turns on LED at col, row
    lc.setLed(dev, 0, 5, true); // turns on LED at col, row
    
    lc.setLed(dev, 1, 1, true); // turns on LED at col, row
    lc.setLed(dev, 1, 6, true); // turns on LED at col, row

    lc.setLed(dev, 2, 0, true); // turns on LED at col, row
    lc.setLed(dev, 2, 7, true); // turns on LED at col, row
    lc.setLed(dev, 2, 2, true); // turns on LED at col, row
    lc.setLed(dev, 2, 5, true); // turns on LED at col, row

    lc.setLed(dev, 3, 0, true); // turns on LED at col, row
    lc.setLed(dev, 3, 7, true); // turns on LED at col, row

    lc.setLed(dev, 4, 0, true); // turns on LED at col, row
    lc.setLed(dev, 4, 7, true); // turns on LED at col, row
    lc.setLed(dev, 4, 2, true); // turns on LED at col, row
    lc.setLed(dev, 4, 5, true); // turns on LED at col, row

    lc.setLed(dev, 5, 0, true); // turns on LED at col, row
    lc.setLed(dev, 5, 3, true); // turns on LED at col, row
    lc.setLed(dev, 5, 4, true); // turns on LED at col, row
    lc.setLed(dev, 5, 7, true); // turns on LED at col, row

    lc.setLed(dev, 6, 1, true); // turns on LED at col, row
    lc.setLed(dev, 6, 6, true); // turns on LED at col, row
    
    lc.setLed(dev, 7, 2, true); // turns on LED at col, row
    lc.setLed(dev, 7, 3, true); // turns on LED at col, row
    lc.setLed(dev, 7, 4, true); // turns on LED at col, row
    lc.setLed(dev, 7, 5, true); // turns on LED at col, row
}

void clearMatrix() {
  lc.clearDisplay(0);// clear screen
  lc.clearDisplay(1);// clear screen
  lc.clearDisplay(2);// clear screen
  lc.clearDisplay(3);// clear screen
}
// ***** LED MATRIX *****
