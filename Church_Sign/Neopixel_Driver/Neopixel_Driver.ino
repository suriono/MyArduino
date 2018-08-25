#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
 
#define PIN_TOP_SIGN 2
#define PIN_BOTTOM_SIGN 3

// 1st row
Adafruit_NeoMatrix matrix1 = Adafruit_NeoMatrix(32, 8, 3, 1, PIN_TOP_SIGN,
  NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_PROGRESSIVE +
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);
// 2nd row
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

DynamicJsonDocument jsonFromText;

//void Neopixel_Initial();
//void Neopixel_Process_Input_Serial(String tmp);
//void Neopixel_Adjust_Brightness();

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Neopixel_Initial();
  Serial.print("width:"); Serial.println(matrix1.width());  
}

// =========================================================

void loop() {
  static unsigned long last_time_brigthness;
  
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
      if(c == '{') {
        isSignText = true;
        SignText = "{";
      } else if ( c == '}') {
        if (isSignText) {
          Neopixel_Process_Input_Serial(SignText);
        }
        
        isSignText = false;
        SignText = "";
      }
    }
    //Serial.print("Incoming Serial:"); Serial.println(incoming_serial);
    
  } else if ( (millis() - last_time_brigthness) > 10000) { // check brightness
    
    last_time_brigthness = millis();
    Neopixel_Adjust_Brightness();
  }
}

