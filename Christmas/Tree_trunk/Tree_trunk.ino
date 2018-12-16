#include <FastLED.h>
#include <ESP8266WiFi.h>
#define NUM_LEDS 7680
#define DATA_PIN 10
#define NUM_LED_ADJ 2    // number of adjacent LEDs

CRGB leds[NUM_LEDS];
CRGB colors[5] = {CRGB::Blue, CRGB::Red, CRGB::Green, CRGB(0,255,255),CRGB(45,205,0)}; // GRB
int pp;


void setup() {
  Serial.begin(57600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  for (byte nn=0 ; nn<NUM_LEDS ; nn++) {
    leds[nn] = CRGB::Black;  // turn all off first
  }
  FastLED.show(); 
}

void loop() {
  static byte nn = 0;
  static int sign = 1;
  //static int pp = 0;

  leds[nn] = CRGB::Black;
  nn = (nn + sign) ; // % NUM_LEDS; remainder not needed since it changes direction
  
  if (nn == NUM_LED_ADJ-2 ) {
    sign = 1;
    pp = (pp + 1) % 5;  // for color changing
    leds[0] = CRGB::Black;
  } else if (nn == (NUM_LEDS-NUM_LED_ADJ+1)) {
    pp = (pp + 1) % 5;  // for color changing
    sign = -1;
    leds[NUM_LEDS-1] = CRGB::Black;
  } else {
    for (byte mm=0 ; mm<NUM_LED_ADJ; mm++) leds[nn+mm*sign] = colors[pp];
  }

  
  FastLED.show(); 
  delay(50);
  

}
