//#include "LedControl.h"
//LedControl lc=LedControl(12,10,11,1);  //  (DIN pin, CLK pin, CS pin, the number of LED matrices)

#include <LedControl.h>
int DIN1 = 10;
int CS1 =  9;
int CLK1 = 8;

LedControl lc1=LedControl(DIN1,CLK1,CS1,1);

int DIN2 = 7;
int CS2 =  6;
int CLK2 = 5;

LedControl lc2=LedControl(DIN2,CLK2,CS2,1);

unsigned long delayTime=800;  // pause between image displays
// the field that shows one image on the LED matrix
byte srce1[] =
{
   B01100110,  //values of the first LED matrix row; 1 - turned on, 0 - turned off LED
   B11111111, //values of the second LED matrix row
   B11111111, //values of the third LED matrix row
   B11111111, //values of the fourth LED matrix row
   B01111110, //values of the fifth LED matrix row 
   B00111100, //values of the sixth LED matrix row 
   B00011000, //values of the seventh LED matrix row
   B00000000 //values of the eight LED matrix row
};
// the field that shows another image on the LED matrix
byte srce2[] =
{
   B00001111,
   B00001111,
   B00001111,
   B00001111,
   B00001111,
   B00001111,
   B00001111,
   B00001111
};
void setup()
{
  lc1.shutdown(0,false);  // turn on the LED matrix
  lc1.setIntensity(0,1);  // set the light intensity (0-15)
  lc1.clearDisplay(0);  // clear the LED matrix display

  lc2.shutdown(0,false);  // turn on the LED matrix
  lc2.setIntensity(0,1);  // set the light intensity (0-15)
  lc2.clearDisplay(0);  // clear the LED matrix display
}
void prikazi_srce1()
{
  for (int i = 0; i < 8; i++) 
  {
    lc1.setRow(0,i,srce1[i]); // display of the elements field, row by row
  }
}
void prikazi_srce2()
{
  for (int j = 0; j < 8; j++) 
  {
    lc2.setRow(0,j,srce2[j]);
  }
}
void loop()
{
    prikazi_srce1(); //show the first image
    delay(delayTime); //wait for delayTime milliseconds
    prikazi_srce2(); //show the second image
    delay(delayTime); //wait for delayTime milliseconds
}
