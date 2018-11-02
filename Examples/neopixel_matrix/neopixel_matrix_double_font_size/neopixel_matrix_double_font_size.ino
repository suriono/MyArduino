#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define PIN_TOP_SIGN 2
/*
Adafruit_NeoMatrix matrix1 = Adafruit_NeoMatrix(32, 8, 2, 2, PIN_TOP_SIGN,
  NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_PROGRESSIVE +
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);
  */
  // Use Arduino Due because SRAM is high, 96KB, Mega is 8KB
  Adafruit_NeoMatrix matrix1 = Adafruit_NeoMatrix(32, 8, 2, 15, PIN_TOP_SIGN,
  NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_ZIGZAG +
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);

void setup() {
 
  matrix1.begin();
  matrix1.setTextWrap(false); 
  matrix1.setBrightness(1);  // from 0 to 255
  matrix1.setTextColor(matrix1.Color(255, 0, 0));
  matrix1.setTextSize(2);
  matrix1.fillScreen(0);  
  matrix1.setCursor(0, 0);

  matrix1.print("K   O");
  //matrix1.print("LOVE never");
 // matrix1.setCursor(0,15);
 // matrix1.setTextSize(1);
 // matrix1.print("K");
  //matrix1.print("fails you");
  
  matrix1.show(); 
  Serial.print("width:"); Serial.println(matrix1.width());  

}

void loop() {
 
}
