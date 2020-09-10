#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>


#define NEOPIXEL_PIN    10

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
  Neomatrix_scrolltext("Happy Saint Patrick's Day" , 0, 250, 0);
  Neomatrix_scrollbitmap("stPatrick", false);

  Neomatrix_scrolltext("Brought to you by" , 0, 0, 250);
  Neomatrix_scrolltext("Design Enablement" , 250, 0, 250);
}
