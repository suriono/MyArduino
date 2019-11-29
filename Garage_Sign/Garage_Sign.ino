#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
 
#define PIN 4   // D2 for NodeMCU
 
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, PIN,
  NEO_MATRIX_BOTTOM    + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);
const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(255, 255, 0),matrix.Color(0, 0, 255), matrix.Color(255, 0, 255), matrix.Color(0, 255, 255), matrix.Color(255, 255, 255)};
WiFiClient client;
 
// =================== Setup ========================

void setup() {
  matrix_init();
  Wifi_Init();
  
}


// ================== Loop ==========================

int x    = matrix.width();
int pass = 0;

void loop() {

 
  
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(F("Happy birthday to me. Thank's Fern"));
 
  if(--x < -200) { // was 30 for 10 character
    x = matrix.width();
 
    if(++pass >= 8) pass = 0;
    matrix.setTextColor(colors[pass]);
  }
  
  matrix.show();
  delay(300);

}
