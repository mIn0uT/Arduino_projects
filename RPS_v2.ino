#include "LedControl.h"
#include "binary.h"

// matrix 1
#define DIN1 8
#define CS1 9
#define CLK1 10
#define DEV 1        //No of Devices
LedControl lc1 = LedControl(DIN1,CLK1,CS1,DEV);

// matrix 2
#define DIN2 5
#define CS2 6
#define CLK2 7
#define DEV 1
LedControl lc2 = LedControl(DIN2,CLK2,CS2,DEV); 

//switches
const int choice_rock 13;
const int choice_paper 12;
const int choice_scissor 11;
// set button reading to int
int rock_state, paper_state, scissor_state;

//7 segment 1
int Aa = A3; 
int Ba = A2; // not sure here
int Ca = A1; // try to change
int Da = A0;

//7 segment 2
int Ab = 1;
int Bb = 2;
int Cb = 3;
int Db = 4;

int user_choice, computer_choice, player_score, comp_score, Result;

// matrix displays
byte P[8] = {0x0c,0x1e,0x33,0x33,0x33,0x33,0xff,0xff};
byte C[8] = {0xc3,0xc3,0xc3,0xc3,0xc3,0xc3,0xff,0xff};

byte rock[8] = {0x30,0x38,0x3c,0x3c,0x3c,0x3c,0x38,0x30};
byte paper[8] = {0xfc,0x82,0xa9,0xa9,0xa9,0xa9,0x81,0xff};
byte scissor[8] = {0x81,0xc3,0x66,0x3c,0x18,0xff,0xa5,0xe7};

byte one[8] = {0x00,0x00,0x03,0xff,0xff,0x00,0x00,0x00};
byte two[8] = {0xc6,0xe3,0xf3,0xdb,0xdb,0xdb,0xcf,0xc6};
byte three[8] = {0x66,0xe7,0xc3,0xdb,0xdb,0xdb,0xff,0x66};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  playerscore=0;
  compscore=0;
  
  // matrix 1 setup
  lc1.shutdown(0,false);
  lc1.setIntensity(0,15);         // Set the brightness (0-15)
  lc1.clearDisplay(0);  
  
  // matrix 2 setup
  lc2.shutdown(0,false);
  lc2.setIntensity(0,15);         // Set the brightness (0-15)
  lc2.clearDisplay(0); 
  
  // buttons setup
  pinMode(choice_rock, INPUT);
  pinMode(choice_paper, INPUT);
  pinMode(choice_scissor, INPUT);
}

void loop() {
  m1_Display(P);
  m2_Display(C);

  //wait for user input on one of three pins (paper, rock, scissors)
  do
  {
    rock_state = digitalRead(choice_rock);
    paper_state = digitalRead(choice_paper);
    scissor_state = digitalRead(choice_scissor);
  }
  while (rock_state == LOW && paper_state== LOW && scissor_state == LOW);
  
  if (choice_paper == HIGH) {
    m1_Display(paper);
    user_choice = 0;
  }
  else if (choice_rock == HIGH) {
    m1_Display(rock);
    user_choice = 1;
  }
  else if (choice_scissor == HIGH) {
    m1_Display(scissor);
    user_choice = 2;
  };

  // set computer choice
  int computer_choice = random(0,3);
  comp_Choice(computer_choice);

  // check who wins
  Result = user_choice - computer_choice
  
  if (Result == -1 || Result == 2)
    { m2_Display(C);
      // add score to computer
      // comp_score ++
      delay (2000);
    }
  else if (Result == 1 || Result == -2)
    { m1_Display(P);
      // add score to player
      // player_score ++ 
      delay (2000);
    }
  else if (Result == 0)
    { m1_Display(P);
      m2_Display(C);
      delay (2000);
    };
  //3s delays before new round
  m1_Display(three);
  m2_Display(three);
  delay (1000);
  m1_Display(two);
  m2_Display(two);
  delay (1000);
  m1_Display(one);
  m2_Display(one);
  delay (1000);
}

//computer choice
void comp_Choice(int ai) {
  switch(ai)
  {
    case 0:
      m2_Display(paper);
      delay(2000);
      break;
    case 1:
      m2_Display(rock);
      delay(2000);
      break;
    case 2:
      m2_Display(scissor);
      delay(2000);
      break;
  }
}

// for matrix 1 displays
void m1_Display(byte RowByte[8]) {
  // write string to MAX
  for (int row = 0; row < 8; row++)
  {
    lc1.setColumn(0, 7 - row, RowByte[row]);
  }
}

// for matrix 2 displays
void m2_Display(byte RowByte[8]) {
  // write string to MAX
  for (int row = 0; row < 8; row++)
  {
    lc2.setColumn(0, 7 - row, RowByte[row]);
  }
}
