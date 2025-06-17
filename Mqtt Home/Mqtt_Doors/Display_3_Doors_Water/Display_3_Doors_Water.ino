#include "EspMQTTClient.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Arduino_JSON.h>
#include "password.h"

#define WATER_ALERT_CONTINUOUS_HOURS 24    // how manu hours of continuous water flow

#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#elif __has_include(<WiFiNINA.h>)
#include <WiFiNINA.h>
#elif __has_include(<WiFi101.h>)
#include <WiFi101.h>
#elif __has_include(<WiFiS3.h>)
#include <WiFiS3.h>
#endif
#include <ESP_Mail_Client.h>

#define TFT_MISO  12    // D6 
#define TFT_LED   5     // D1  
#define TFT_SCK   4     // D2
#define TFT_MOSI  13    // D7 MOSI
#define TFT_DC    0     // D3 FLASH
#define TFT_RESET 2     // D4
#define TFT_CS    15    // D8 CS
#define BUZZER    14    // D5 
#define TFT_SCREEN_OFF_COUNTDOWN 2400    // countdown to turn the screen off to save power
#define MAX_MSG   8     // maximum character for the message on the right side of TFT

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT esp_mail_smtp_port_465 // port 465 is not available for Outlook.com
// hidden for security #define AUTHOR_EMAIL "xxxxxxxxxxxxxx"
// hidden for security #define AUTHOR_PASSWORD "xxxxxxxxxxxxxxxx"
// hidden for security #define RECIPIENT_EMAIL "xxxxxxxxxxxxxxxxxx"
SMTPSession smtp;
Session_Config config;
void smtpCallback(SMTP_Status status);

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RESET, TFT_MISO);
EspMQTTClient client( WIFI_SSID, WIFI_PASSWD, "192.168.0.122", MQTT_USER, MQTT_PASSWD, "uzmqtt",1883); // passwords are hidden


JSONVar myJSON;
int     countDown_Alarm = 0;
int     countDown_screenOFF = TFT_SCREEN_OFF_COUNTDOWN;
int     Door_Number     = 0;        // 0=freezer, 1=top fridge 2=bottom fridge

void setup() {
  Serial.begin(57600);
  pinMode(TFT_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(TFT_LED, HIGH);
  tft_init();

  smtp_init();

}

void loop() {
  static unsigned long last_time = millis();
  static String last_String;
  String new_String;
  uint16_t color_text;
  static boolean buzz_toggle = false;

  client.loop();

  if ( (millis() - last_time) > 1000) {
    last_time = millis();
    
    if (countDown_Alarm > 1) {
      countDown_Alarm -= 1;
      tft_countDown();
      
    } else if ( countDown_Alarm == 1) {
      tft_countDown();
      if (buzz_toggle) {         // timer expires, start buzzing
        tone(BUZZER, 600, 250);
        buzz_toggle = false;
      } else {
        noTone(BUZZER);
        buzz_toggle = true;
      }
    } else {
      countDown_screenOFF -= 1;
    }
    if ( countDown_screenOFF < 0) {
       tft_screen_OnOff(false);
    }
  }
  
}
