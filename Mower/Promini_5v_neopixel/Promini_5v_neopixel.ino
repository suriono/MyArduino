#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <FastLED.h>

#define NEOPIXEL_PIN    9
#define LED_STRING_PIN  10
#define EVA_PIN         11      // when Eva picture detected
#define NUM_LEDS        50

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, NEOPIXEL_PIN,
  NEO_MATRIX_BOTTOM    + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

CRGB leds[NUM_LEDS];

// ==================== Setup =================================

void setup() {
  Serial.begin(9600);
  Neopixel_Initial();
  FastLED.addLeds<NEOPIXEL, LED_STRING_PIN>(leds, NUM_LEDS);
  for (byte nn=0 ; nn<NUM_LEDS ; nn++) {
    leds[nn] = CRGB::Black;  // turn all off first
  }
  for (byte nn=NUM_LEDS-12 ; nn<NUM_LEDS ; nn++) {
    leds[nn] = CRGB::Green;
  }
  FastLED.show(); 
  pinMode(EVA_PIN, INPUT_PULLUP);      // when Eva picture detected
}

// ==================== Loop ===================================
int x = matrix.width();
int pass = 0;
const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0),matrix.Color(0, 0, 255), matrix.Color(255, 0, 255), matrix.Color(0, 255, 255), matrix.Color(255, 255, 0), matrix.Color(255, 255, 255)};
 
void loop() {
  static byte leds_brightness = 0;
  static boolean leds_upbright = true;
  static byte color_rotation = 0;
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(F("I am Wall-E. Where is Eva?"));
 
  if(--x < -145) { // was 30 for 10 character
    x = matrix.width();
 
    if(++pass >= 6) pass = 0;
    matrix.setTextColor(colors[pass]);
  }

  if (digitalRead(EVA_PIN)) {
    
    if (leds_brightness > 200) {
      leds_upbright = false;
    } else if (leds_brightness < 20) {
      leds_upbright = true;
      color_rotation = (color_rotation+1) % 3;
      for (byte nn=NUM_LEDS-12 ; nn<NUM_LEDS ; nn++) {
        switch (color_rotation) {
          case 1:
            leds[nn] = CRGB::Green;
            break;
          case 2:
            leds[nn] = CRGB::Blue;
            break;
          default:
            leds[nn] = CRGB::Red;
            break;
        }
      }
    }
    leds_brightness += 5 * (leds_upbright - !leds_upbright);
  
  } else {  // when Eva picture detected
    for (byte nn=NUM_LEDS-12 ; nn<NUM_LEDS ; nn++) {
      leds[nn] = CRGB::White;
    }
    leds_brightness = 220;
  }
  FastLED.setBrightness(leds_brightness);

  running_LED();
  
  matrix.show();
  FastLED.show();
  delay(30);
}

// ============================================

void Neopixel_Initial() {
  delay(2000);   // so no big surge of current at the beginning
  matrix.begin(); 
  matrix.setTextWrap(true); 
  matrix.setBrightness(10);  // from 0 to 255
  matrix.setTextColor(matrix.Color(155, 0, 0));
  matrix.fillScreen(0);
  matrix.setCursor(0, 0);
  matrix.print(F("Howdy"));
  matrix.show();
  //Text1 = "Happy";
  
}
