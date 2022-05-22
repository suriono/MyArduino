#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>
//#include <Fonts/FreeSerif9pt7b.h>


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
  int randn = random(0,767);
  byte red = 0;
  byte green = 0;
  byte blue = 0;
  byte comb;

  if (randn < 256) {
      red = 255 - randn;
      green = randn;
  } else if (randn < 511) {
      green = 511 - randn;
      blue = randn - 256;
  } else {
      red = 767 - randn;
      blue = randn - 512;
  }
  
  Neomatrix_scrolltext("Welcome" ,red, green, blue);
 // colorcombination = !colorcombination;
  /*
  byte red = random(0,255);
  byte green = random(0,255);
  byte blue = random(0,255);
  byte combination = random(0,4);

  switch (combination) {
    case 0:
      Neomatrix_scrolltext("Welcome to 13934 Fallbrook Way" ,red, green, blue);
      break;
    case 1:
       Neomatrix_scrolltext("Welcome to 13934 Fallbrook Way" ,red, green, 0);
       break;
    case 2:
       Neomatrix_scrolltext("Welcome to 13934 Fallbrook Way" ,red, 0, blue);
       break;
    case 3:
       Neomatrix_scrolltext("Welcome to 13934 Fallbrook Way", 0, green, blue);
       break;
    default:
       Neomatrix_scrolltext("Welcome to 13934 Fallbrook Way", red/2, green, blue/2);
  }
  */

  /*
 
  Neomatrix_scrolltext("O say, can you see", 200, 0, 0);
  Neomatrix_scrolltext("By the dawn's early light", 0, 0, 200);
  Neomatrix_scrolltext("What so proudly we hail", 200, 0, 0);
  Neomatrix_scrolltext("At the twilight's last gleaming", 0, 0, 200);

  //static byte count = 0;
  Neomatrix_scroll_picture(0, 13, 30);

  Neomatrix_scrolltext("Whose broad stripes and bright stars", 200, 0, 0);
  Neomatrix_scrolltext("Through the perilous fight", 0, 0, 200);
  Neomatrix_scrolltext("O'er the ramparts we watch", 200, 0, 0);
  Neomatrix_scrolltext("Were so gallantly streaming", 0, 0, 200);

  Neomatrix_scroll_picture(0, 13, 30);

  Neomatrix_scrolltext("And the rocket's red glare", 200, 0, 0);
  Neomatrix_scrolltext("The bombs bursting in air", 0, 0, 200);
  Neomatrix_scrolltext("Gave proof through the night", 200, 0, 0);
  Neomatrix_scrolltext("That our flag was still there", 0, 0, 200);

  Neomatrix_scroll_picture(0, 13, 30);

  Neomatrix_scrolltext("O say, does that star-spangled banner yet wave", 200, 0, 0);
  Neomatrix_scrolltext("O'er the land of the free", 0, 0, 200);
  Neomatrix_scrolltext(".........................", 200, 0, 0);
  Neomatrix_scrolltext("And the home of the brave", 0, 0, 200);

  Neomatrix_scroll_picture(0, 13, 30);
  */
  //Neomatrix_scroll_picture(0, 35, 32);
  
  
}
