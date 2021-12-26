#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>


#define NEOPIXEL_PIN    4  // D2 for NodeMCU

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, NEOPIXEL_PIN,
  NEO_MATRIX_BOTTOM  + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

// ============= Setup ==========================

void setup() {
  Serial.begin(57600);
  Neopixel_Initial();
  //Neomatrix_scroll_picture(1);
  //uint16_t mcolor1 = matrix.Color(200,200,0);
  //matrix.drawPixel(10, 10, mcolor1); 
  //matrix.show();
}

// ============== Loop ===========================

void loop() {
//  Neomatrix_scrolltext("Merry Christmas" , 0, 250, 0);
//  Neomatrix_scrolltext("Happy Diwali" , 232, 142, 19);
 // Neomatrix_scrolltext("Happy New Year" , 232, 19, 213);
 // Neomatrix_scrolltext("Happy Hanukkah" , 0, 0, 250);
  //Neomatrix_scrolltext("Las Posadas" , 60, 180, 56);
  //Neomatrix_scrolltext("Happy Kwanzaa" , 232, 71, 19);
  
  //Neomatrix_scrollbitmap("star", false);

  //Neomatrix_scrolltext("Happy Halloween" , 32, 232, 20);
  //Neomatrix_scrolltext("Happy ThanksGiving" , 175, 40, 120);
  //Neomatrix_scrolltext("Design Enablement" , 250, 0, 250);

  //static byte count = 0;
  Neomatrix_scroll_picture(0);
  //for(byte nn = 0; nn < 3; nn++) {
   //  
  //}

  //Neomatrix_scrolltext("Bite me ---- " , 175, 40, 10);
  //count = (count + 1) % 3;
  //Serial.println(count);
}
