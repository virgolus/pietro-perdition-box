#include <Wire.h>
#include "pitches.h"
#include "LedControl.h" //  need the library
#include "SoftwareSerial.h"
#include <avr/sleep.h>//this AVR library contains the methods that controls the sleep modes

// General vars
int games[] = {1, 2, 3};
int currentGameIndex = 0;
unsigned long lastButtonPressMillis = millis();
unsigned long standbyWaitMillis = 60000;
unsigned long loseWaitMillis = 10000;
int changeGamePin = 3;
boolean ingame = false;
boolean gameChanged = true;

// ***** MEMORY *****
int button[] = {768, 510, 164, 91}; //The four button input pins
int ledpin[] = {8, 9, 12, 13};  // LED pins
int buttonstate = 0;  // button state checker
int sequenceLength = 2;
int randomArray[4];
int randomNotesArray[4];
int inputArray[4];
int btn_down_duration = 600;
int seq = 0;
// ***** MEMORY *****

// ***** TALPA *****
int whereIs;
// ***** TALPA *****

// ***** LED MATRIX *****
int DIN_PIN = 6;      // data in pin
int CS_PIN = 7;       // load (CS) pin
int CLK_PIN = 5;      // clock pin
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 4); //
// ***** LED MATRIX *****

// ******* BUZZER ******** //
int buzzPin = 2;
int notes[] = {NOTE_C4, NOTE_G4, NOTE_D4, NOTE_E4};  // notes
// ******* BUZZER ******** //

void setup()
{
  Serial.begin(115200);

  // init Leds
  for (int x = 0; x < 4; x++) // LED pins are outputs
  {
    pinMode(ledpin[x], OUTPUT);
  }

  randomSeed(analogRead(0)); //Added to generate "more randomness" with the randomArray for the output function

  initMatrix();
  delay(500);
  drawInitGameMatrix();
  delay(2000);

  // Change game button
    // changeGame
  pinMode(changeGamePin, INPUT_PULLUP);
  attachInterrupt(1, gameSwitch, CHANGE); 
}

void loop()
{
  if (games[currentGameIndex] == 1) {
    startMemoryGame();
  } else if (games[currentGameIndex] == 2) {
    startTalpaGame();
  } else if (games[currentGameIndex] == 3) {
    startTalpaContinuousGame();
  }
}

void startMemoryGame() {

  Serial.println("MEMORY GAME");
  if (gameChanged) {
    drawInitMemory();
    delay(1000);
  }
  ingame = true;

  Serial.println("random array:");
  for (int y = 0; y < sequenceLength; y++)
  {
    // Generate random sequence
    int rnd = random(4);
    randomArray[y] = rnd;
    Serial.print(randomArray[y]);

    digitalWrite(ledpin[rnd], HIGH);

    // Buzzer&notes
    int note = notes[rnd];
    buzz(note, 500);
    randomNotesArray[y] = note;

    //delay(500);
    digitalWrite(ledpin[rnd], LOW);
    delay(200);
  }

  memoryInput();
}

void memoryInput() { //Function for allowing user input and checking input against the generated array
  
  while (ingame) {

    checkStandbyDelay();

    int btnVal = analogRead(A0);

    if (btnVal > 25)
    { 
      //Checking for button push
      int btnNum = getButtonNumber(btnVal);

      if (btnNum == 999) {
        continue;
      }


      Serial.print("button pressed: ");
      Serial.println(btnNum);

      lastButtonPressMillis = millis();

      inputArray[seq] = btnNum;

      if (inputArray[seq] != randomArray[seq]) { //Checks value input by user and checks it against
        lose();                             //the value in the same spot on the generated array
      } else {

        // led up and buzzer
        digitalWrite(ledpin[btnNum], HIGH);
        buzz(randomNotesArray[seq], btn_down_duration);
        digitalWrite(ledpin[btnNum], LOW);

        ++seq;
        if (seq == sequenceLength) {
          win();
        }
      }
    }
  }
}

void startTalpaGame() {

  Serial.println("TALPA GAME");
  if (gameChanged) {
    drawInitTalpa();
    delay(1000);
  }
  ingame = true;

  // Generate random sequence
  int rnd = random(0, 4);
  whereIs = rnd;

  drawSmile(3 - rnd);
  // Buzzer&notes
  int note = notes[rnd];
  buzz(note, 500);

  clearMatrix();

  talpaInput();
}

void talpaInput() { //Function for allowing user input and checking input against the generated array

  while (ingame) {

    checkStandbyDelay();

    int btnVal = analogRead(A0);

    if (btnVal > 25)
    { //Checking for button push
      int btnNum = getButtonNumber(btnVal);

      if (btnNum == 999) {
        continue;
      }

      Serial.print("button pressed: ");
      Serial.println(btnNum);
      
      lastButtonPressMillis = millis();
  
      digitalWrite(ledpin[btnNum], HIGH);
      buzz(randomNotesArray[seq], 200);
      delay(300);
      digitalWrite(ledpin[btnNum], LOW);

      if ((whereIs) == btnNum) { //Checks value input by user and checks it against
        break;
      } else {
        lose();
      }

    }
  }
}

void startTalpaContinuousGame() {

  Serial.println("TALPA CONTINUOUS GAME");

  ingame = true;

  while (ingame) {
    // Generate random sequence
    int rnd = random(0, 4);
    whereIs = rnd;

    drawSmile(3 - rnd);
    // Buzzer&notes
    int note = notes[rnd];
    buzz(note, 500);

    int btnVal = analogRead(A0);

    if (btnVal > 10)
    { //Checking for button push
      int btnNum = getButtonNumber(btnVal);

      if (btnNum == 999) {
        continue;
      }

      Serial.print("button pressed: ");
      Serial.println(btnNum);

      digitalWrite(ledpin[btnNum], HIGH);
      buzz(randomNotesArray[seq], 100);
      digitalWrite(ledpin[btnNum], LOW);

      clearMatrixPart(3 - btnNum);
    } else {
      delay(1000);
    }
  }
}

void gameSwitch() {
  Serial.println("GAME SWITCH!!!!");
    ingame = false;
    gameChanged = true;
    lastButtonPressMillis = millis();
    if (currentGameIndex == 1) {
      currentGameIndex = 0;
    } else if (currentGameIndex == 0) {
      currentGameIndex = 1;
    }
}

// TODO: If no activity for standbyWaitMillis, power off
void checkIfPowerDown() {
  long currentMillis = millis();

 if (currentMillis - lastButtonPressMillis >= standbyWaitMillis) {
  
    sleep_enable();//Enabling sleep mode
    //attachInterrupt(0, wakeUp, LOW);//attaching a interrupt to pin d2
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);//Setting the sleep mode, in our case full sleep
    digitalWrite(LED_BUILTIN,LOW);//turning LED off
    //delay(1000); //wait a second to allow the led to be turned off before going to sleep
    sleep_cpu();//activating sleep mode
    //Serial.println("just woke up!");//next line of code executed after the interrupt 
    //digitalWrite(LED_BUILTIN,HIGH);//turning LED on
 }
}

// If no activity for loseWaitMillis, display lose() and restart game
void checkStandbyDelay() {
  long currentMillis = millis();

 if (currentMillis - lastButtonPressMillis >= loseWaitMillis) {
    lastButtonPressMillis = millis();
    lose();
 }
}

void win() { //Function used if the player fails to match the sequence
  resetVars();

  draw4Smiles();

  for (int x = 0; x < 8; x++) {
    for (int led = 0; led < 4; led++) {
      digitalWrite(ledpin[led], HIGH);

      buzz(NOTE_D4, 50);

      digitalWrite(ledpin[led], LOW);
    }
    for (int led = 4; led > 0; led--) {
      digitalWrite(ledpin[led], HIGH);

      buzz(NOTE_E4, 50);

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
    buzz(NOTE_C4, 300);
    clearMatrix();
    delay(100);
  }

  delay(2000);
}

void resetVars() {
  ingame = false;
  gameChanged = false;
  seq = 0;
  memset(inputArray, 0, sizeof(inputArray));
  memset(randomArray, 0, sizeof(randomArray));
}

// ***** LED MATRIX *****
void drawInitGameMatrix() {
  Serial.println("INIT MATRIX");
  for (int l = 0; l < 8; l++) {
    for (int dev = 3; dev >= 0; dev--) {
      for (int c = 0; c < 8; c++) {
        lc.setLed(dev, l, c, true); // turns on LED at col, row
        delay(3);
        if (l > 0) {
          lc.setLed(dev, l - 1, c, false); // turns on LED at col, row
        }
      }
    }
  }

  clearMatrix();
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

void drawInitMemory() {
  for (int dev = 0; dev < 4; dev++)
  {
    drawSmile(dev);
    buzz(notes[random(4)], 100);
    clearMatrixPart(dev);
  }
}

void drawInitTalpa() {
  drawArrow(0);
  buzz(notes[random(4)], 100);
  clearMatrixPart(0);
  drawArrow(2);
  buzz(notes[random(4)], 100);
  clearMatrixPart(2);
  drawArrow(1);
  buzz(notes[random(4)], 100);
  clearMatrixPart(1);
  drawArrow(3);
  buzz(notes[random(4)], 100);
  clearMatrix();
  clearMatrixPart(3);
}

void drawArrow(int dev) {

  lc.setLed(dev, 1, 3, true); // turns on LED at col, row
  lc.setLed(dev, 1, 4, true); // turns on LED at col, row

  lc.setLed(dev, 2, 3, true); // turns on LED at col, row
  lc.setLed(dev, 2, 4, true); // turns on LED at col, row

  lc.setLed(dev, 3, 3, true); // turns on LED at col, row
  lc.setLed(dev, 3, 4, true); // turns on LED at col, row

  lc.setLed(dev, 4, 1, true); // turns on LED at col, row
  lc.setLed(dev, 4, 2, true); // turns on LED at col, row
  lc.setLed(dev, 4, 3, true); // turns on LED at col, row
  lc.setLed(dev, 4, 4, true); // turns on LED at col, row
  lc.setLed(dev, 4, 5, true); // turns on LED at col, row
  lc.setLed(dev, 4, 6, true); // turns on LED at col, row

  lc.setLed(dev, 5, 2, true); // turns on LED at col, row
  lc.setLed(dev, 5, 3, true); // turns on LED at col, row
  lc.setLed(dev, 5, 4, true); // turns on LED at col, row
  lc.setLed(dev, 5, 5, true); // turns on LED at col, row

  lc.setLed(dev, 6, 3, true); // turns on LED at col, row
  lc.setLed(dev, 6, 4, true); // turns on LED at col, row
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

void clearMatrixPart(int dev) {
  lc.clearDisplay(dev);// clear screen
}

void initMatrix() {
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

void buzz(unsigned int nota, long durata) {
  tone(buzzPin, nota, durata);
  delay(durata);
  noTone(buzzPin);
}

// Get btnNum from analog input. return 999 if value is not in rangre
int getButtonNumber(int btnVal) {
  Serial.println(btnVal);
  int btnNum = 999;
  if (btnVal > 650 && btnVal < 820) {
    btnNum = 0;
  } else if (btnVal > 450 && btnVal < 570) {
    btnNum = 1;
  } else if (btnVal > 150 && btnVal < 220) {
    btnNum = 2;
  } else if (btnVal > 30 && btnVal < 140) {
    btnNum = 3;
  }

  return btnNum;
}
