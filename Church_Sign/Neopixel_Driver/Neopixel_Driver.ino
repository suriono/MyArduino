#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
 
#define PIN_TOP_SIGN 2
#define TILE_COLUMNS 6
#define TILE_ROWS    5
#define NEO_RED      9109504

 // Use Arduino Due because SRAM is high, 96KB, Mega is 8KB
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, TILE_COLUMNS, TILE_ROWS, PIN_TOP_SIGN,
  NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_ZIGZAG +
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);

boolean isCursorEnable = false;
uint16_t cursorNum=0;
uint32_t cursorColor_last;

void setup() {
  Serial.begin(57600);
  Serial1.begin(57600);
  
  Neopixel_Initial();
  cursorColor_last = matrix.getPixelColor(0);
}

// =========================================================

void loop() {
  
  //static unsigned long last_time_brightness;
  
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
      } else if ( c == '<') {
        if (isSignText) {
          SignText = SignText.substring(0, SignText.length()-1);
          Neopixel_Process_Input_Serial(SignText);
        }
        isSignText = false;
        SignText = "";
      }
    }
    //last_time_brightness = millis(); // so brightness doesn't update immediately
  //} else if ( (millis() - last_time_brightness) > 10000) { // check brightness  
  //  last_time_brightness = millis();
    // Neopixel_Adjust_Brightness();
  }
  
}
