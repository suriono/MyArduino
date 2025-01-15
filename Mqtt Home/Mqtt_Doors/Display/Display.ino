#include "EspMQTTClient.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "password.h"

#define TFT_MISO  12    // D6 
#define TFT_LED   5     // D1  
#define TFT_SCK   4     // D2
#define TFT_MOSI  13    // D7 MOSI
#define TFT_DC    0     // D3 FLASH
#define TFT_RESET 2     // D4
#define TFT_CS    15    // D8 CS
#define MAGNETIC_PIN 14 // D5 RXD0

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RESET, TFT_MISO);

EspMQTTClient client( WIFI_SSID, WIFI_PASSWD, "192.168.0.122", MQTT_USER, MQTT_PASSWD, "uzmqtt",1883); // passwords are hidden

void setup() {
  Serial.begin(57600);
  pinMode(TFT_LED, OUTPUT);
  pinMode(MAGNETIC_PIN, INPUT_PULLUP);
  digitalWrite(TFT_LED, HIGH);

 
  tft_init();
  
}

void loop() {
  static unsigned long last_time = millis();
  static String last_String;
  String new_String;
  uint16_t color_text;

  client.loop();

//  if ( (millis() - last_time) > 200) {
//    last_time = millis();
//
//    if ( digitalRead(MAGNETIC_PIN) ) {
//      new_String = "OPEN";
//      color_text = tft.color565(250, 0 , 0);
//    } else {
//      new_String = "CLOSE";
//      color_text = tft.color565(0, 250 , 0);
//    }
//
//    if (new_String != last_String) {
//      tft.setCursor(150, 0);
//      tft.setTextColor(ILI9341_BLACK);
//      tft.print(last_String);
//      tft.setCursor(150, 0);
//      tft.setTextColor(color_text);    
//      tft.print(new_String);
//      last_String = new_String;
//    }
//  }
  
}
