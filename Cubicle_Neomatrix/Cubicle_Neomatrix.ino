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
}

// ============== Loop ===========================

void loop() {
 /*
 Send_Text("O say, can you see");
  Send_Text("By the dawn's early light");
  Send_Text("What so proudly we hail");
  Send_Text("At the twilight's last gleaming");

  //static byte count = 0;
  Neomatrix_scroll_picture(0, 13, 30);

  Send_Text("Whose broad stripes and bright stars");
  Send_Text("Through the perilous fight");
  Send_Text("O'er the ramparts we watch");
  Send_Text("Were so gallantly streaming");

  Neomatrix_scroll_picture(0, 13, 30);

  Send_Text("And the rocket's red glare");
  Send_Text("The bombs bursting in air");
  Send_Text("Gave proof through the night");
  Send_Text("That our flag was still there");

  Neomatrix_scroll_picture(0, 13, 30);

  Send_Text("O say, does that star-spangled banner yet wave");
  Send_Text("O'er the land of the free");
  Send_Text("And the home of the brave");

  Neomatrix_scroll_picture(0, 13, 30);
  Neomatrix_scroll_picture(0, 13, 30);
  Neomatrix_scroll_picture(0, 13, 30);

  Send_Text("Happy Independence Day ......");

  Neomatrix_scroll_picture(0, 13, 30);
  Neomatrix_scroll_picture(0, 13, 30);
  Neomatrix_scroll_picture(0, 13, 30);
  */

  Neomatrix_scroll_picture(0, 35, 32);
  Neomatrix_scrolltext("Made" ,0, 0, 200);
  Neomatrix_scrolltext("in" ,200, 200, 200);
  Neomatrix_scrolltext("America" ,200, 0, 0);
  Neomatrix_scroll_picture2(0, 13, 30);
}

// ============== Send Text =================

void Send_Text(String inputstr) {

  int randn = random(0,767);
  byte red = 0;
  byte green = 0;
  byte blue = 0;

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
  
  Neomatrix_scrolltext(inputstr ,red, green, blue);
  
}
