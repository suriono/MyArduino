void running_Flag(int timedelay, int ntimes, String type) {
  unsigned int nn = 0;
  int sign = 1;
  int last_nn;

  for(int n=0 ; n < (NUM_LEDS - 4)*ntimes ; n++) {
    
    if (nn == (NUM_LEDS-5)) sign = -1;
    if (nn == 0 ) sign = 1;
    
  
    nn = (nn + sign) % (NUM_LEDS-4);

    // WS2811 red and green are in reverse
    leds[nn] = CRGB::Blue;
    leds[nn+sign] = CRGB(100,100,100); // white
    leds[nn+2*sign] = CRGB::Green;
    leds[nn+3*sign] = CRGB(100,100,100); // white
    
    FastLED.show(); 
    delay(timedelay);
    leds[nn] = CRGB::Black; // turn off previous one
    leds[nn+sign] = CRGB::Black; // turn off previous one
    leds[nn+2*sign] = CRGB::Black; // turn off previous one
    leds[nn+3*sign] = CRGB::Black; // turn off previous one
    //FastLED.show();
    
  }
}

