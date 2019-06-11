#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
 
#define PIN_TOP_SIGN 2
#define TILE_COLUMNS 6
#define TILE_ROWS    5
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
  matrix.setBrightness(10);  // from 0 to 255
  matrix.setTextSize(5);
  matrix.fillScreen(0); 
  matrix.setTextColor(matrix.Color(0, 0, 200));
  matrix.show();
}

int x = 0;
int nbright = 1;
int color_cycle = 0;
int bright_inc = 10;

void loop() {
  matrix.fillScreen(0);
  matrix.setCursor(x, 3);
  matrix.print(F("Welcome to"));
  //matrix.show();
  //matrix.setCursor(x, 20);
  //matrix.print(F("River Hills"));
  matrix.setBrightness(nbright);
  nbright = nbright % 50 + bright_inc;

  if (nbright < bright_inc*2) {
    color_cycle = color_cycle%3 + 1;
    matrix.setTextColor(matrix.Color(150*(color_cycle==3), 150*(color_cycle==2), 150*(color_cycle==1)));
  }

  if(++x > 150) { // was 30 for 10 character
    x = 0;
  }
  
  matrix.show();
  delay(100);

}
