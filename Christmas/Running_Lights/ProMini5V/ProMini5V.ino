#include <FastLED.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

SoftwareSerial SoftSerial(9, 8); // RX, TX not used
#define NUM_LEDS 50  // 7680
#define DATA_PIN 10
#define NUM_LED_ADJ 2    // number of adjacent LEDs

CRGB leds[NUM_LEDS];
byte Num_Colors = 6;
CRGB Colors[6]  = {CRGB::Blue, CRGB::Red, CRGB::Green, CRGB(0,155,100),CRGB(45,205,0),CRGB(200,0,100) }; // GRB
int Color_Set  = 1;
int  Delay_Time = 100;
int  Mode = 1;              // 0=uni direction, 1=bounce
int  Adjacent = 4;
//String InStr;

// ============================================================

void setup() {
  Serial.begin(57600);
  SoftSerial.begin(57600);
  LED_init(); 
  
  Serial.println("Read EEPROM");
  char readee;
  String eestr;
  for (byte een=0; een<30; een++) {
    readee = EEPROM.read(een);
    if (int(readee) > 0) {
      eestr += readee;
      Serial.print(readee);
    }
  }
  if (eestr.length() > 10) {
    JSON_process(eestr);
  }
  Serial.println();
}

// =============================================================

void loop() {
  static String InStr;
  char ch;
  
  if (SoftSerial.available()) {
      ch = SoftSerial.read();
      if (ch == '{') {
        InStr = ch;
      } else {
        InStr += ch;
      }
      if (ch == '}') {
        JSON_process(InStr);
      }
  } else {
    //Serial.print(Mode);
    switch(Mode) {
      case 0:
        LED_unidirection();
        break;
      case 1:
        LED_bounce();
        break;
      case 2:
        LED_uni_mix();
        break;
    }
    FastLED.show(); 
    delay(Delay_Time);
   
  }
  delay(1);
}
