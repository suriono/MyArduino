#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
 
#define PIN_TOP_SIGN 2
#define TILE_COLUMNS 1
#define TILE_ROWS    5
#define NEO_RED      9109504

 // Use Arduino Due because SRAM is high, 96KB, Mega is 8KB
//Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, 6, 5, PIN_TOP_SIGN,
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
  
  Neopixel_Initial("hello", 10);
  cursorColor_last = matrix.getPixelColor(0);
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
      if(c == '>') {
      //if(c == '{') {
        isSignText = true;
        //SignText = "{";
      //} else if ( c == '}') {
      } else if ( c == '<') {
        if (isSignText) {
          SignText = SignText.substring(0, SignText.length()-1);
          Neopixel_Process_Input_Serial(SignText);
          //Serial.println(SignText);
        }
        
        isSignText = false;
        SignText = "";
      }
    }
    //Serial.print("Incoming Serial:"); Serial.println(incoming_serial);
    
  } else if ( (millis() - last_time_brigthness) > 10000) { // check brightness  
    last_time_brigthness = millis();
    /*
    int np=0;
    for (int row=0 ; row<8 ; row++) {
      for (int col=0 ; col<32 ; col++) {
        Serial.print(row); Serial.print(","); Serial.print(col);Serial.print(",");
        Serial.println(matrix.getPixelColor(np));
        np++;
      }
    }
    */
    //Neopixel_Adjust_Brightness();
  } else { // if (isCursorEnable) {
    showCursor(isCursorEnable);
  }
  
}
