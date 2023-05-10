int aPin1 = 2; // connect to pin 14 on first SN74LS47N
int bPin1 = 3; // connect to pin 13 on first SN74LS47N
int cPin1 = 4; // connect to pin 12 on first SN74LS47N
int dPin1 = 5; // connect to pin 11 on first SN74LS47N
int digitPin1 = 6; // connect to pin 15 on first SN74LS47N
int enablePin1 = 7; // connect to pin 10 on first SN74LS47N

int aPin2 = 8; // connect to pin 14 on second SN74LS47N
int bPin2 = 9; // connect to pin 13 on second SN74LS47N
int cPin2 = 10; // connect to pin 12 on second SN74LS47N
int dPin2 = 11; // connect to pin 11 on second SN74LS47N
int digitPin2 = 12; // connect to pin 15 on second SN74LS47N
int enablePin2 = 13; // connect to pin 10 on second SN74LS47N

int button1 = A0; // connect button for player 1 to analog pin 0
int button2 = A1; // connect button for player 2 to analog pin 1

int score1 = 0; // player 1 score
int score2 = 0; // player 2 score

void setup() {
  pinMode(aPin1, OUTPUT);
  pinMode(bPin1, OUTPUT);
  pinMode(cPin1, OUTPUT);
  pinMode(dPin1, OUTPUT);
  pinMode(digitPin1, OUTPUT);
  pinMode(enablePin1, OUTPUT);
  
  pinMode(aPin2, OUTPUT);
  pinMode(bPin2, OUTPUT);
  pinMode(cPin2, OUTPUT);
  pinMode(dPin2, OUTPUT);
  pinMode(digitPin2, OUTPUT);
  pinMode(enablePin2, OUTPUT);
  
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(button1) == LOW) {
    score1++;
    displayDigit(score1, aPin1, bPin1, cPin1, dPin1, digitPin1, enablePin1);
    delay(200); // debounce
  }
  
  if (digitalRead(button2) == LOW) {
    score2++;
    displayDigit(score2, aPin2, bPin2, cPin2, dPin2, digitPin2, enablePin2);
    delay(200); // debounce
  }
}

void displayDigit(int digit, int aPin, int bPin, int cPin, int dPin, int digitPin, int enablePin) {
  digitalWrite(enablePin, LOW);
  digitalWrite(digitPin, HIGH);
  digitalWrite(aPin, bitRead(digit, 0));
  digitalWrite(bPin, bitRead(digit, 1));
  digitalWrite(cPin, bitRead(digit, 2));
  digitalWrite(dPin, bitRead(digit, 3));
  digitalWrite(enablePin, HIGH);
}
