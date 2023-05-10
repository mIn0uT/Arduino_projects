/*
BCD to 7-segment-display
playground2014.wordpress.com
*/

int dtime;
byte ziffer[11][4] = { { 0,0,0,0 }, // = 0
                       { 0,0,0,1 }, // = 1
                       { 0,0,1,0 }, // = 2
                       { 0,0,1,1 }, // = 3
                       { 0,1,0,0 }, // = 4
                       { 0,1,0,1 }, // = 5
                       { 0,1,1,0 }, // = 6
                       { 0,1,1,1 }, // = 7
                       { 1,0,0,0 }, // = 8
                       { 1,0,0,1 }, // = 9
                       { 1,1,1,1 }  // = define as off - more information refer the datasheet 74LS47N
                     };

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  digitalWrite(2, 1);
  digitalWrite(3, 1);
  digitalWrite(4, 1);
  digitalWrite(5, 1);
  delay(2000);
}

void ziffer_schreiben(byte digit) {
  digitalWrite(2,ziffer[digit][3]);
  digitalWrite(3,ziffer[digit][2]);
  digitalWrite(4,ziffer[digit][1]);
  digitalWrite(5,ziffer[digit][0]);
}

void loop() {
  dtime=700 ;
  for (int i=0; i<=9; ++i) {
    ziffer_schreiben(i) ;
    delay(dtime);
  }

  dtime=300 ;
  for (int i=8; i>=0; --i) {
    ziffer_schreiben(i) ;
    delay(dtime);
  }
}
