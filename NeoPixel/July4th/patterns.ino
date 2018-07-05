// ============= Bouncing back and forth ====================

void Running(int timedelay, int ntimes, String type) {
  unsigned int nn = 0;
  int sign = 1;
  int last_nn;

  for(int n=0 ; n < NUM_LEDS * ntimes ; n++) {
    if (type.equals("bounce")) {
      if (nn == (NUM_LEDS-1)) sign = -1;
      if (nn == 0 ) sign = 1;
    } else {
      sign = 1;
    }
  
    nn = (nn + sign) % NUM_LEDS;

    // WS2811 red and green are in reverse
    if (nn % 3 == 1) {
      leds[nn] = CRGB::Green; // is green for WS2811
    } else if (nn % 3 == 2) {
      leds[nn] = CRGB(100,100,100); // white
    } else {
      leds[nn] = CRGB::Blue; // is red for WS2811
    }
    
    FastLED.show(); 
    delay(timedelay);
    leds[nn] = CRGB::Black; // turn off previous one
  }
}

