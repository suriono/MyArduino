//int nn;
//byte pp;
void LED_init() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  for (byte nn=0 ; nn<NUM_LEDS ; nn++) {
    leds[nn] = CRGB::Black;  // turn all off first
  }
  FastLED.show();
}

// ==================================

void LED_unidirection() {   
  static int  nn  = NUM_LEDS;
  int  sign = -1;
  static byte pp   = 0;
  
  leds[nn] = CRGB::Black;
  nn = nn + sign;
  if (nn < 0) {
      nn = NUM_LEDS + Adjacent - 2;
      if ( Color_Set == Num_Colors) {
        pp = (pp + 1) % Num_Colors;  // for color changing
      } else {
        pp = Color_Set;
      }
  }
  for (byte mm=0 ; mm<Adjacent; mm++) {
      if ((nn+mm*sign > -1) && (nn+mm*sign < NUM_LEDS)) {
        leds[nn+mm*sign] = Colors[pp];
      }
  }
  Serial.print(Mode);
}

// ==================================

void LED_uni_mix() {    
  static int  kk  = NUM_LEDS;
  int  sign = -1;
  static byte pp   = 0;
  
  leds[kk] = CRGB::Black;
  kk = kk + sign;
  if (kk < 0) {
      kk = NUM_LEDS + Adjacent - 2;
  } else if ((kk % (NUM_LEDS/Num_Colors)) == 4) {
    //if ( Color_Set == Num_Colors) {
      pp = (pp + 1) % Num_Colors;  // for color changing
   // } else {
    //  pp = Color_Set;
    //}
  }
  for (byte mm=0 ; mm<Adjacent; mm++) {
      if ((kk+mm*sign > -1) && (kk+mm*sign < NUM_LEDS)) {
        leds[kk+mm*sign] = Colors[pp];
      }
  }
   Serial.print(Mode);
}

// ==================================

void LED_bounce() {   // Mode = 1
  static int  nn  = NUM_LEDS;
  static int  sign = -1;
  static byte pp   = 0;

  nn = nn + sign;
  if (nn == -Adjacent) {  
    if ( Color_Set == Num_Colors) {
      pp = (pp + 1) % Num_Colors;  // for color changing
    } else {
      pp = Color_Set;
    }
    sign = 1;
    nn = 0;
  } else if (nn == NUM_LEDS+Adjacent-1) {
    if ( Color_Set == Num_Colors) {
      pp = (pp + 1) % Num_Colors;  // for color changing
    } else {
      pp = Color_Set;
    }
    sign = -1;
    nn = NUM_LEDS - 1;
  } else if ((nn-sign*Adjacent)>-1 && (nn-sign*Adjacent)<NUM_LEDS) {
    leds[nn-sign*Adjacent] = CRGB::Black; 
  }
  if (nn >-1 && nn < NUM_LEDS) {
    //Serial.print (pp);
    leds[nn] = Colors[pp]; 
  }
   Serial.print(Mode);
}
