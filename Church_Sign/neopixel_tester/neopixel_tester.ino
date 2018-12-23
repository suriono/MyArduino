#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
 
#define PIN_TOP_SIGN 2
#define TILE_COLUMNS 2
#define TILE_ROWS    1
#define NEO_RED      9109504

 // Use Arduino Due because SRAM is high, 96KB, Mega is 8KB
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, TILE_COLUMNS, TILE_ROWS, PIN_TOP_SIGN,
  NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_ZIGZAG +
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);
 
void setup() {
  Serial.begin(57600);
  matrix.begin();
  matrix.setTextWrap(false); 
  matrix.setBrightness(50);  // from 0 to 255
  matrix.setTextSize(1);
  matrix.fillScreen(0); 
  matrix.setTextColor(matrix.Color(0, 155, 0));
  matrix.show();
}

int x = 0;

void loop() {
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(F("Welcome to"));
  //matrix.show();
  //matrix.setCursor(x, 20);
  //matrix.print(F("River Hills"));

  if(++x > 60) { // was 30 for 10 character
    x = 0;
  }
  
  matrix.show();
  delay(10);

}
