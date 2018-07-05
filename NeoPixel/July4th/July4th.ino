#include <FastLED.h>
#define NUM_LEDS 50
#define DATA_PIN 6

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(57600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

}

void loop() {

  Serial.println("Running");
  //Running(20, 2, "bounce");
  //Running(20, 2, "oneway");

  for(uint8_t nn=5 ; nn>0 ; nn--) {
    flag_dim(nn);
  }

  roll_Flag(100, 30);

  Clear();

  running_Flag(50, 4, "bounce");

}
