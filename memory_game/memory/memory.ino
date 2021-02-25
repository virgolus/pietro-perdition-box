#include <Wire.h>

int button[] = {2, 3, 4, 5}; //The four button input pins
int ledpin[] = {10, 11, 12, 13};  // LED pins
int buttonstate = 0;  // button state checker
int sequenceLength = 3;
int randomArray[4];
int inputArray[4];
boolean ingame = true;
int seq = 0;


void setup()
{
  /*
    lcd.setBacklightPin(3,POSITIVE);
    lcd.setBacklight(LOW);
    lcd.begin(16, 2);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Your Score: 0");
    lcd.setCursor(0, 0);
    lcd.print("High Score: 0");
  */
  Serial.begin(9600);

  // init Leds
  for (int x = 0; x < 4; x++) // LED pins are outputs
  {
    pinMode(ledpin[x], OUTPUT);
  }

  // init Buttons
  for (int x = 0; x < 4; x++)
  {
    pinMode(button[x], INPUT_PULLUP); // button pins are inputs
    //digitalWrite(button[x], HIGH);  // enable internal pullup; buttons start in high position; logic reversed
  }

  randomSeed(analogRead(0)); //Added to generate "more randomness" with the randomArray for the output function

  /*
    for (int led = 0; led <= 4; led++) {
      digitalWrite(ledpin[led], HIGH);

      delay(150);
      // stop for the next note:
      //speakerpin.stop();
      digitalWrite(ledpin[led], LOW);

    }
    for (int led = 4; led >= 0; led--) {
      digitalWrite(ledpin[led], HIGH);

      delay(150);
      // stop for the next note:
      //speakerpin.stop();
      digitalWrite(ledpin[led], LOW);

    }

    delay(1000);
  */
}

void loop()
{
  // Generate random sequence
  ingame = true;
  Serial.println("random array:");
  for (int y = 0; y < sequenceLength; y++)
  {
    int rnd = random(sequenceLength);
    randomArray[y] = rnd;
    Serial.print(randomArray[y]);
    Serial.println("");
    digitalWrite(ledpin[rnd], HIGH);
    delay(400);
    digitalWrite(ledpin[rnd], LOW);
    delay(100);
  }

  input();
}

void input() { //Function for allowing user input and checking input against the generated array

  while (ingame) {

    for (int btn = 0; btn < 4; btn++)
    {
      buttonstate = digitalRead(button[btn]);

      if (buttonstate == LOW)
      { //Checking for button push

        Serial.print("button pressed: ");
        Serial.println(btn);
        Serial.print("seq: ");
        Serial.println(seq);

        digitalWrite(ledpin[btn], HIGH);
        delay(300);
        digitalWrite(ledpin[btn], LOW);
        inputArray[seq] = btn;

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
}

void win() { //Function used if the player fails to match the sequence
  resetVars();

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
}

void lose() { //Function used if the player fails to match the sequence
  resetVars();

  for (int y = 0; y < 3; y++)
  { //Flashes lights for failure

    digitalWrite(ledpin[0], HIGH);
    digitalWrite(ledpin[1], HIGH);
    digitalWrite(ledpin[2], HIGH);
    digitalWrite(ledpin[3], HIGH);
    delay(300);
    digitalWrite(ledpin[0], LOW);
    digitalWrite(ledpin[1], LOW);
    digitalWrite(ledpin[2], LOW);
    digitalWrite(ledpin[3], LOW);
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
