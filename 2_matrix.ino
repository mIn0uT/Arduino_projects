//#include "LedControl.h"
//LedControl lc=LedControl(12,10,11,1);  //  (DIN pin, CLK pin, CS pin, the number of LED matrices)

#include <LedControl.h>
// pins for 1st matrix
int DIN1 = 10;
int CS1 =  9;
int CLK1 = 8;
LedControl lc1=LedControl(DIN1,CLK1,CS1,1);

// pins for 2nd matrix
int DIN2 = 7;
int CS2 =  6;
int CLK2 = 5;
LedControl lc2=LedControl(DIN2,CLK2,CS2,1);

unsigned long delayTime=800;  // pause between image displays

// output shapes in matrix
byte rock[] = {0x1c,0x3e,0x7f,0xff,0xff,0xfe,0x7c,0x38};
// binary version
// {B00011100,
// B00111110,
// B01111111,
// B11111111,
// B11111111,
// B11111110,
// B01111100,
// B00111000}

byte paper[] = {0x3c,0x42,0x81,0xbd,0x81,0xbd,0x81,0xff};
// binary values
// {B00111100,
// B01000010,
// B10000001,
// B10111101,
// B10000001,
// B10111101,
// B10000001,
// B11111111}

byte scissor[] = {0xe1,0xa2,0xe4,0x18,0x18,0xe4,0xa2,0xe1};
// binary version
// {B11100001,
// B10100010,
// B11100100,
// B00011000,
// B00011000,
// B11100100,
// B10100010,
// B11100001}

void setup()
{
  lc1.shutdown(0,false);  // turn on the LED matrix
  lc1.setIntensity(0,1);  // set the light intensity (0-15)
  lc1.clearDisplay(0);  // clear the LED matrix display

  lc2.shutdown(0,false);  // turn on the LED matrix
  lc2.setIntensity(0,1);  // set the light intensity (0-15)
  lc2.clearDisplay(0);  // clear the LED matrix display
}

void print_rock()
{
  for (int i = 0; i < 8; i++) 
  {
    lc1.setRow(0,i,rock[i]); // change lc1 to lc2 to show in 2nd matrix
  }
}

void print_paper()
{
  for (int i = 0; i < 8; i++) 
  {
    lc1.setRow(0,i,paper[i]);
  }
}

void print_scissor()
{
  for (int i = 0; i < 8; i++) 
  {
    lc1.setRow(0,i,scissor[i]);
  }
}

void loop()
{
    print_rock(); //show the first image
    delay(delayTime); //wait for delayTime milliseconds
    print_paper(); //show the second image
    delay(delayTime); //wait for delayTime milliseconds
    print_scissor(); //show the third image
    delay(delayTime); //wait for delayTime milliseconds
}
