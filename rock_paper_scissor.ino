#include "LedControl.h"
#include "binary.h"

// Matrix 1
#define DIN1 8
#define CS1 9
#define CLK1 10
#define DEV 1
LedControl lc1 = LedControl(DIN1, CLK1, CS1, DEV);

// Matrix 2
#define DIN2 5
#define CS2 6
#define CLK2 7
LedControl lc2 = LedControl(DIN2, CLK2, CS2, DEV);

// Switches
#define choice_rock 13
#define choice_paper 12
#define choice_scissor 11
//int rock_state, paper_state, scissor_state;

// 7-segment display 1
int clockPin1 = A0;
int latchPin1 = A1;
int dataPin1 = A2;

// 7-segment display 2
int clockPin2 = A3;
int latchPin2 = A4;
int dataPin2 = A5;

// Common anode
int num[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

int user_choice, computer_choice, player_score,comp_score, Result;

// Matrix displays
byte P[8] = {0x00,0x7c,0x66,0x66,0x7c,0x60,0x60,0x00};
byte C[8] = {0x00,0x7e,0x7e,0x60,0x60,0x7e,0x7e,0x00};
byte W[8] = {0x00, 0x42, 0x42, 0x5a, 0x7e, 0x66, 0x42, 0x00};
byte L[8] = {0x00, 0x60, 0x60, 0x60, 0x60, 0x7e, 0x7e, 0x00};
byte T[8] = {0x00,0x42,0x24,0x18,0x18,0x24,0x42,0x00};

byte rock[8] = {0x00,0x00,0x3c,0x7e,0xff,0xff,0x00,0x00};
byte paper[8] = {0x3f,0x41,0x81,0xbd,0x81,0xbd,0x81,0xff};
byte scissor[8] = {0xe1,0xa2,0xe4,0x38,0x38,0xe4,0xa2,0xe1};

byte one[8] = {0x38,0x38,0x18,0x18,0x18,0x18,0x18,0x18};
byte two[8] = {0x7e,0xff,0x83,0x1e,0x3c,0x60,0xff,0xff};
byte three[8] = {0x7e,0xff,0x83,0x1e,0x1e,0x83,0xff,0x7e};

void setup() {
  Serial.begin(9600);
  
  lc1.shutdown(0, false);
  lc1.setIntensity(0, 7);
  lc1.clearDisplay(0);

  lc2.shutdown(0, false);
  lc2.setIntensity(0, 7);
  lc2.clearDisplay(0);

  pinMode(choice_rock, INPUT);
  pinMode(choice_paper, INPUT);
  pinMode(choice_scissor, INPUT);

  pinMode(clockPin1, OUTPUT);
  pinMode(latchPin1, OUTPUT);
  pinMode(dataPin1, OUTPUT);

  pinMode(clockPin2, OUTPUT);
  pinMode(latchPin2, OUTPUT);
  pinMode(dataPin2, OUTPUT);

  player_score = 0;
  scoringPlayer(player_score);
  comp_score = 0;
  scoringComputer(comp_score);
}

void loop() {

  m1_Display(P);
  m2_Display(C);

  while (digitalRead(choice_rock) == LOW && digitalRead(choice_paper) == LOW && digitalRead(choice_scissor) == LOW);

  if (digitalRead(choice_rock) == HIGH) {
    m1_Display(rock);
    user_choice = 0;
    //delay(1000);
  } else if (digitalRead(choice_paper) == HIGH) {
    m1_Display(paper);
    user_choice = 1;
    //delay(1000);
  } else if (digitalRead(choice_scissor) == HIGH) {
    m1_Display(scissor);
    user_choice = 2;
    //delay(1000);
  }

  int computer_choice = random(0, 3);
  comp_Choice(computer_choice);

  scoringSystem(user_choice, computer_choice);

  // Whoever first to score 3 wins
  if (player_score == 3) {
    // show player wins
    m1_Display(P);
    m2_Display(W);
    player_score = 0;
    scoringPlayer(player_score);
    comp_score = 0;
    scoringComputer(comp_score);
    delay(2000);
  } else if (comp_score == 3) {
    // show computer wins
    m1_Display(W);
    m2_Display(C);
    player_score = 0;
    scoringPlayer(player_score);
    comp_score = 0;
    scoringComputer(comp_score);
    delay(2000);
  }

  m1_Display(three);
  m2_Display(three);
  delay(1000);
  m1_Display(two);
  m2_Display(two);
  delay(1000);
  m1_Display(one);
  m2_Display(one);
  delay(1000);
}

void scoringPlayer(int score) {
  digitalWrite(latchPin1, LOW);
  shiftOut(dataPin1, clockPin1, MSBFIRST, num[score]);
  digitalWrite(latchPin1, HIGH);
}

void scoringComputer(int score) {
  digitalWrite(latchPin2, LOW);
  shiftOut(dataPin2, clockPin2, MSBFIRST, num[score]);
  digitalWrite(latchPin2, HIGH);
}

void comp_Choice(int ai) {
  switch (ai) {
    case 0:
      m2_Display(rock);
      delay(2000);
      break;
    case 1:
      m2_Display(paper);
      delay(2000);
      break;
    case 2:
      m2_Display(scissor);
      delay(2000);
      break;
  }
}

void m1_Display(byte RowByte[8]) {
  for (int row = 0; row < 8; row++) {
    lc1.setColumn(0, 7 - row, RowByte[row]);
  }
}

void m2_Display(byte RowByte[8]) {
  for (int row = 0; row < 8; row++) {
    lc2.setColumn(0, 7 - row, RowByte[row]);
  }
}

void scoringSystem(int u, int c) {
  switch (u) {
    case 0:
      switch (c) {
        case 0:
          m1_Display(T);
          m2_Display(T);
          break;
        case 1:
          m2_Display(W);
          m1_Display(L);
          comp_score++;
          scoringComputer(comp_score);
          break;
        case 2:
          m1_Display(W);
          m2_Display(L);
          player_score++;
          scoringPlayer(player_score);
          break;
      }
      break;

    case 1:
      switch (c) {
        case 0:
          m1_Display(W);
          m2_Display(L);
          player_score++;
          scoringPlayer(player_score);
          break;
        case 1:
          m1_Display(T);
          m2_Display(T);
          break;
        case 2:
          m2_Display(W);
          m1_Display(L);
          comp_score++;
          scoringComputer(comp_score);
          break;
      }
      break;

    case 2:
      switch (c) {
        case 0:
          m2_Display(W);
          m1_Display(L);
          comp_score++;
          scoringComputer(comp_score);
          break;
        case 1:
          m1_Display(W);
          m2_Display(L);
          player_score++;
          scoringPlayer(player_score);
          break;
        case 2:
          m1_Display(T);
          m2_Display(T);
          break;
      }
      break;
  }

  delay(2000);
}
