// BLUE 01
const int l_blue_1 = 10;
const int in_blue_1 = 2;

// GREEN 01
const int l_green_1 = 11;
const int in_green_1 = 3;

// YELLOW 01
const int l_yellow_1 = 12;
const int in_yellow_1 = 4;

// RED 01
const int l_red_1 = 13;
const int in_red_1 = 5;

// init buttons
int b_blue_1 = 0;
int b_green_1 = 0;
int b_yellow_1 = 0;
int b_red_1 = 0;



void setup() {

  pinMode(l_blue_1, OUTPUT);
  pinMode(in_blue_1, INPUT_PULLUP);
  pinMode(l_green_1, OUTPUT);
  pinMode(in_green_1, INPUT_PULLUP);
  pinMode(l_yellow_1, OUTPUT);
  pinMode(in_yellow_1, INPUT_PULLUP);
  pinMode(l_red_1, OUTPUT);
  pinMode(in_red_1, INPUT_PULLUP);

  Serial.begin(115200);         // Start the Serial communication to send messages to the computer

  Serial.print("Pistacchio 01 Started");
}

// the loop function runs over and over again forever
void loop() {

  // BLUE 01
  b_blue_1 = digitalRead(in_blue_1);
  if (b_blue_1 == LOW) {
    // turn LED on:
    digitalWrite(l_blue_1, HIGH);
    Serial.println("b_blue pressed");
  } else {
    // turn LED off:
    digitalWrite(l_blue_1, LOW);
  }

  // GREEN 01
  b_green_1 = digitalRead(in_green_1);

  if (b_green_1 == LOW) {
    // turn LED on:
    digitalWrite(l_green_1, HIGH);
    Serial.println("b_green pressed");
  } else {
    // turn LED off:
    digitalWrite(l_green_1, LOW);
  }

  // YELLOW 01
  b_yellow_1 = digitalRead(in_yellow_1);

  if (b_yellow_1 == LOW) {
    // turn LED on:
    digitalWrite(l_yellow_1, HIGH);
    Serial.println("b_yellow pressed");
  } else {
    // turn LED off:
    digitalWrite(l_yellow_1, LOW);
  }

  // RED 01
  b_red_1 = digitalRead(in_red_1);

  if (b_red_1 == LOW) {
    // turn LED on:
    digitalWrite(l_red_1, HIGH);
    Serial.println("b_red pressed");
  } else {
    // turn LED off:
    digitalWrite(l_red_1, LOW);
  }
}
