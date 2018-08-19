#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
 
#define PIN_TOP_SIGN 2
#define PIN_BOTTOM_SIGN 3

Adafruit_NeoMatrix matrix1 = Adafruit_NeoMatrix(32, 8, 3, 1, PIN_TOP_SIGN,
  NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_PROGRESSIVE +
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);

Adafruit_NeoMatrix matrix2 = Adafruit_NeoMatrix(32, 8, 3, 1, PIN_BOTTOM_SIGN,
  NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_PROGRESSIVE +
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);

/* for single tile
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, 2, 1, PIN,
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG +
  NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_ZIGZAG,
   NEO_GRB + NEO_KHZ800);
*/
const uint16_t colors[] = {
  matrix1.Color(255, 0, 0), matrix1.Color(0, 255, 0), matrix1.Color(255, 255, 0),matrix1.Color(0, 0, 255), matrix1.Color(255, 0, 255), matrix1.Color(0, 255, 255), matrix1.Color(255, 255, 255)};
 
String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  //inputString.reserve(200);  // reserve 200 bytes for the inputString:

  Neopixel_Initial();
  
  Serial.print("width:"); Serial.println(matrix1.width());  
}

void loop() {
  
  static String SignText;
  static boolean isSignText = false;
  if (Serial1.available()) {
    String incoming_serial;
    while (Serial1.available()) {
      char c = Serial1.read();

      incoming_serial += c;
      if (isSignText) {  // actual sign text
        SignText += c;
      }
      if(c == '>') {
        isSignText = true;
        SignText = "";
      } else if ( c == '<') {
        isSignText = false;
        Serial.print("Sign:"); Serial.println(SignText.substring(0, SignText.length()-2));
        Neopixel_Colorful_Text(SignText.substring(0, SignText.length()-1),"River Hills");
        SignText = "";
      }
    }
    Serial.println(incoming_serial);
  }
}

