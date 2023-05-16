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
#define sw1 13
#define sw2 12
#define sw3 11

//
int sw1_val;
int sw2_val;
int sw3_val;

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

bool user_input = false;

int player, comp, playerscore, compscore;

byte P[] = {0x0c,0x1e,0x33,0x33,0x33,0x33,0xff,0xff};
byte C[] = {0xc3,0xc3,0xc3,0xc3,0xc3,0xc3,0xff,0xff};

byte rock[] = {0x30,0x38,0x3c,0x3c,0x3c,0x3c,0x38,0x30};
byte paper[] = {0xfc,0x82,0xa9,0xa9,0xa9,0xa9,0x81,0xff};
byte scissor[] = {0x81,0xc3,0x66,0x3c,0x18,0xff,0xa5,0xe7};

byte one[] = {0x00,0x00,0x03,0xff,0xff,0x00,0x00,0x00};
byte two[] = {0xc6,0xe3,0xf3,0xdb,0xdb,0xdb,0xcf,0xc6};
byte three[] = {0x66,0xe7,0xc3,0xdb,0xdb,0xdb,0xff,0x66};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  playerscore=0;
  compscore=0;

  lc1.shutdown(0,false);
  lc1.setIntensity(0,15);         // Set the brightness (0-15)
  lc1.clearDisplay(0);  

  lc2.shutdown(0,false);
  lc2.setIntensity(0,15);         // Set the brightness (0-15)
  lc2.clearDisplay(0); 

  pinMode(sw1, INPUT);
  pinMode(sw2, INPUT);
  pinMode(sw3, INPUT);
}

void loop() {
  // // Show Text
  // Serial.println("1. Rock");
  // Serial.println("2. Paper");
  // Serial.println("3. Scissors");
  // Serial.println("Select your choice: ");

  // // Read Input
  // while (Serial.available()) {
  //   Serial.read();
  // }
  // while (Serial.available() == 0) {
  // }
  // int player = Serial.parseInt();

  // comp = random(1,4);
  // comp_display(comp);
  // Serial.println("");

  // if ((player == 1 && comp == 2) || (player == 2 && comp == 3)||(player == 3 && comp == 1)){
  //   playerscore++;
  // } else if ((player == 1 && comp == 3) || (player == 2 && comp == 1)||(player == 3 && comp == 2)) {
  //   compscore++;
  // }
  
  // Serial.print("Player Score: ");
  // Serial.print(playerscore);
  // Serial.print(" | Computer Score: ");
  // Serial.println(compscore);
  // Serial.println();

  print_P();
  print_C();
  delay(3000);
  player_rock();
  comp_rock();
  delay(3000);
  player_paper();
  comp_paper();
  delay(3000);
  player_scissor();
  comp_scissor();
  delay(3000);
  lc1.clearDisplay(0);
  lc2.clearDisplay(0);
  delay(5000);

  sw1_val = digitalRead(sw1);
  sw2_val = digitalRead(sw2);
  sw3_val = digitalRead(sw3);

  // while loop to wait for user input
  while (user_input != true) {
    if (sw1_val == HIGH && sw2_val == LOW && sw3_val == LOW)
    {
      player_rock();
      user_input = true;
      player = 1;
    }
    //PAPER
    else if(sw1_val == LOW && sw2_val == HIGH && sw3_val == LOW)
    {
      player_paper();
      user_input = true;
      player = 2;
    }
    //scissors
    else if(sw1_val == LOW && sw2_val == LOW && sw3_val == HIGH)
    {
      player_scissor();
      user_input = true;
      player = 3;
    }
    else{
      lc1.clearDisplay(0);
    }
  }

  // after user choice generate comp choice 
  // comp = random(1,3);
  // comp_choice(comp);

  // check who wins
  // if ((player == 1 && comp == 2) || (player == 2 && comp == 3)||(player == 3 && comp == 1)){
  //   playerscore++;
  // } else if ((player == 1 && comp == 3) || (player == 2 && comp == 1)||(player == 3 && comp == 2)) {
  //   compscore++;
  // }

}



void comp_display(int comp){
  switch(comp){
    case 1:
      Serial.println("Computer: Rock");
      break;
    case 2:
      Serial.println("Computer: Paper");
      break;
    case 3:
      Serial.println("Computer: Scissors");
      break;
  }
}

// for matrix 1 displays
void print_P() {
  for (int j = 0; j < 8; j++) 
  {
    lc1.setRow(0,j,P[j]);
  }
}

void player_rock() {
  for (int j = 0; j < 8; j++) 
  {
    lc1.setRow(0,j,rock[j]);
  }
}

void player_paper() {
  for (int j = 0; j < 8; j++) 
  {
    lc1.setRow(0,j,paper[j]);
  }
}

void player_scissor() {
  for (int j = 0; j < 8; j++) 
  {
    lc1.setRow(0,j,scissor[j]);
  }
}

void print_one() {
  for (int j = 0; j < 8; j++) 
  {
    lc1.setRow(0,j,one[j]);
  }
}

void print_two() {
  for (int j = 0; j < 8; j++) 
  {
    lc1.setRow(0,j,two[j]);
  }
}

void print_three() {
  for (int j = 0; j < 8; j++) 
  {
    lc1.setRow(0,j,three[j]);
  }
}

// for matrix 2 displays
void print_C() {
  for (int j = 0; j < 8; j++) 
  {
    lc2.setRow(0,j,C[j]);
  }
}

void comp_rock() {
  for (int j = 0; j < 8; j++) 
  {
    lc2.setRow(0,j,rock[j]);
  }
}

void comp_paper() {
  for (int j = 0; j < 8; j++) 
  {
    lc2.setRow(0,j,paper[j]);
  }
}

void comp_scissor() {
  for (int j = 0; j < 8; j++) 
  {
    lc2.setRow(0,j,scissor[j]);
  }
}

//computer
void comp_choice(int ai) {
  //computer input
 
  switch(ai)
  {
    case 1:
    //c == 'r';
    comp_rock();
    break;

    case 2:
    comp_paper();
    break;

    case 3:
    comp_scissor();
    break;
  }
}
