#include <FastLED.h>
#define NUM_LEDS 50
#define DATA_PIN 6

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

}

void loop() {
  static int nn = 0;
  static int sign = 1;

  if (nn == (NUM_LEDS-1)) sign = -1;
  if (nn == 0 ) sign = 1;

  leds[nn] = CRGB::Black;
  
  nn = (nn + sign) % NUM_LEDS;

  // WS2811 red and green are in reverse
  if (nn % 3 == 1) {
     leds[nn] = CRGB::Green; // is green for WS2811
  } else if (nn % 3 == 2) {
     leds[nn] = CRGB(100,100,100);
  } else {
//     leds[nn] = CRGB::Green; // is red for WS2811
     leds[nn] = CRGB::Blue; // is red for WS2811
  }

  
  FastLED.show(); 
  delay(300);

}
