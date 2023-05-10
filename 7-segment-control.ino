int aPin = A0; // connect to pin 14 on SN74LS47N
int bPin = A1; // connect to pin 13 on SN74LS47N
int cPin = A2; // connect to pin 12 on SN74LS47N
int dPin = A3; // connect to pin 11 on SN74LS47N
int digitPin = A4; // connect to pin 15 on SN74LS47N
int enablePin = A5; // connect to pin 10 on SN74LS47N

int outputs[] = { // array of outputs from 1 to 9
  0b00000010, // 1
  0b01101111, // 2
  0b00100100, // 3
  0b00110001, // 4
  0b01010011, // 5
  0b01001001, // 6
  0b00011000, // 7
  0b00000001, // 8
  0b00010000  // 9
};

void setup() {
  pinMode(aPin, OUTPUT);
  pinMode(bPin, OUTPUT);
  pinMode(cPin, OUTPUT);
  pinMode(dPin, OUTPUT);
  pinMode(digitPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
}

void loop() {
  for (int i = 0; i < 9; i++) {
    displayDigit(outputs[i]);
    delay(1000);
  }
}

void displayDigit(byte digit) {
  digitalWrite(enablePin, LOW);
  digitalWrite(digitPin, HIGH);
  digitalWrite(aPin, bitRead(digit, 0));
  digitalWrite(bPin, bitRead(digit, 1));
  digitalWrite(cPin, bitRead(digit, 2));
  digitalWrite(dPin, bitRead(digit, 3));
  digitalWrite(enablePin, HIGH);
}
