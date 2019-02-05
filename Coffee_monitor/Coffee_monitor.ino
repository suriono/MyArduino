
#include <LiquidCrystal.h>

const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
const int bright_pin = 3;  // goes to V0 to substitute potentiometer

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define MIN_VALUE 10000

void setup() {
  //Serial.begin(57600);
  lcd.begin(16, 2);
  pinMode(bright_pin, OUTPUT);
  analogWrite(bright_pin, 130); // potentio equivalent around 2.6V
  
}

void loop() {
  static int nn = 1;
  static boolean toggleleft = true, toggleright = true;

  lcd.clear();
  lcd.setCursor(0,0);
  
  nn *= 2 ; // %10000;
  if (nn == 0) nn = 1;

  int left  = nn;             // left  coffee value
  int right = nn + 10;        // right coffee value
  String leftstr  = String(left)  + " %";
  String rightstr = String(right) + " %";

  String lefthead  = "Left";
  String righthead = "Right";
  
  if ( left < MIN_VALUE ) {
    toggleleft = !toggleleft;
    if (toggleleft) lefthead = "";
  } else {
     toggleleft = true;
  }
  if ( right < MIN_VALUE ) {
    toggleright = !toggleright;
    if (toggleright) righthead = "";
  } else {
     toggleright = true;
  }
  printlcd(lefthead, righthead);

  lcd.setCursor(0,1);
  printlcd(leftstr, rightstr);
  
  delay(1000);
}

void printlcd(String left, String right) {
  String coffeestr = left;
  for (byte i=0 ; i< 16-(left.length()+right.length()); i++) {
    coffeestr += " ";
  }
  coffeestr += right;
  lcd.print(coffeestr);
}
