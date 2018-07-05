void flag_dim(int dimdelay) {

  set_Flag(0);
  
  for (uint8_t nn=0 ; nn < 200 ; nn++) {
    FastLED.show(200-nn);
    delay(dimdelay);
  }
  FastLED.setBrightness(200);
}

// ============== roll the flag =================

void roll_Flag(int rolldelay, int ntimes) {
  for (uint8_t nn=0 ; nn<ntimes ; nn++) {
    set_Flag(nn);
    delay(rolldelay);
  }
}

// ================= set flag ===================

void set_Flag(int nshift) {
  int nn;

  FastLED.setBrightness(100);

  for (uint8_t mm=0 ; mm < NUM_LEDS; mm++) {
    nn = (mm + nshift ) % NUM_LEDS;
    Serial.println(nn);
    
    if (mm < NUM_LEDS / 2) {
      if (mm%2) {
        leds[nn] = CRGB::Blue;
      } else {
        leds[nn] = CRGB(150,150,150); // white
      }
    } else {
      if (mm%2) {
        leds[nn] = CRGB::Green;
      } else {
        leds[nn] = CRGB(150,150,150); // white
      }
    }
  }
  FastLED.show();
}

// ================== Clear all ==================

void Clear() {
  for (uint8_t nn=0 ; nn < NUM_LEDS; nn++) {
     leds[nn] = CRGB::Black;
  }
  FastLED.show();
}

