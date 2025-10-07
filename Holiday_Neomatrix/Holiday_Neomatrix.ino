#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>
#include <Arduino_JSON.h>
#include "certs.h"

X509List cert(cert_DigiCert_Global_Root_CA);
String   weather_message;

#define NEOPIXEL_PIN    4  // D2 for NodeMCU
#define NCOLUMNS        64 // number of pixel columns
#define CHAR_WIDTH      6  // font width

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(NCOLUMNS, 8, NEOPIXEL_PIN,
  NEO_MATRIX_TOP  + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + 
  NEO_MATRIX_ZIGZAG, NEO_GRB + NEO_KHZ800);

const char* host = "OTA-FRONT-NEOMATRIX";

const int ESP_BUILTIN_LED = 2;

byte red_random = 0;
byte green_random = 0;
byte blue_random = 0;

String Scroll_Text[2] = {"13934 Fallbrook Way", " ... "}; // for two words

// ============================== setup ============================================

void setup() {
  Serial.begin(57600);

  Serial.println("Booting");
  WiFi.mode(WIFI_STA);

  wifi_begin(); // contain WiFi.begin("xxx","xxxx"); but hidden for security

  ArduinoOTA.setHostname(host);
  ArduinoOTA.onStart([]() { // switch off all the PWMs during upgrade
  });

  ArduinoOTA.onEnd([]() { // do a fancy thing with our board led at end
  });

  ArduinoOTA.onError([](ota_error_t error) {
    (void)error;
    ESP.restart();
  });

  /* setup the OTA server */
  ArduinoOTA.begin();
  Serial.println("Ready");

  Neopixel_Initial();
  
  weather_message = get_Weather();
  delay(10000);
  
}

// =============== Loop ======================

void loop() {
   static unsigned long last_time = millis();
   ArduinoOTA.handle();
   String update_time;

   // Weahter forecast
 //  update_time = "Updated " + String((millis()-last_time)/1000) + " seconds ago from api.weather.gov : ";
//   Send_Text(update_time + weather_message);
 //  if ( (millis()-last_time) >    900000) {     // weather forecast
 //     weather_message = get_Weather();
 //     last_time = millis();
 //  }
    Neomatrix_scroll_picture2(0 ,  26, 32,100); // (xoffset, row, column, delay)
    Neomatrix_scroll_picture2(24,  26, 32,100); // (xoffset, row, column, delay)
    Neomatrix_scroll_picture2(8 ,  26, 32,100); // (xoffset, row, column, delay)
    Neomatrix_scroll_picture2(32,  26, 32,100); // (xoffset, row, column, delay)
    Neomatrix_scroll_picture2(16 , 26, 32,100); // (xoffset, row, column, delay)
  
   //Send_Text("13934 Fallbrook Way");
  //  Neomatrix_scroll_picture2(0,  13, 30,100); // (xoffset, row, column, delay)
//  Send_Text("O say, can you see");
//    Neomatrix_scroll_picture2(24, 13, 30, 100); // (xoffset, row, column, delay)
//  Send_Text("By the dawn's early light");
//    Neomatrix_scroll_picture2(8, 13, 30, 100); // (xoffset, row, column, delay)
//  Send_Text("What so proudly we hail");
//    Neomatrix_scroll_picture2(32, 13, 30, 100); // (xoffset, row, column, delay)
//  Send_Text("At the twilight's last gleaming");
//    Neomatrix_scroll_picture2(16, 13, 30, 100); // (xoffset, row, column, delay)
//
//  Send_Text("Whose broad stripes and bright stars");
//    Neomatrix_scroll_picture2(0,  13, 30,100); // (xoffset, row, column, delay)
//  Send_Text("Through the perilous fight");
//    Neomatrix_scroll_picture2(24, 13, 30, 100); // (xoffset, row, column, delay)
//  Send_Text("O'er the ramparts we watch");
//    Neomatrix_scroll_picture2(8, 13, 30, 100); // (xoffset, row, column, delay)
//  Send_Text("Were so gallantly streaming");
//    Neomatrix_scroll_picture2(32, 13, 30, 100); // (xoffset, row, column, delay)
//
//  Send_Text("And the rocket's red glare");
//    Neomatrix_scroll_picture2(16, 13, 30, 100); // (xoffset, row, column, delay)
//  Send_Text("The bombs bursting in air");
//   Neomatrix_scroll_picture2(0,  13, 30,100); // (xoffset, row, column, delay)
//  Send_Text("Gave proof through the night");
//    Neomatrix_scroll_picture2(24, 13, 30, 100); // (xoffset, row, column, delay)
//  Send_Text("That our flag was still there");
//    Neomatrix_scroll_picture2(8, 13, 30, 100); // (xoffset, row, column, delay)
//
//  Send_Text("O say, does that star-spangled banner yet wave");
//    Neomatrix_scroll_picture2(32, 13, 30, 100); // (xoffset, row, column, delay)
//  Send_Text("O'er the land of the free");
//    Neomatrix_scroll_picture2(16, 13, 30, 100); // (xoffset, row, column, delay)
//  Send_Text("And the home of the brave");
   //Neomatrix_scroll_picture2(0,  13, 30,100); // (xoffset, row, column, delay)

  // Neomatrix_scrolltext_random_eachstep("Happy Father's Day");

  //Send_Text("Happy Memorial Day");
  
  //Neomatrix_scroll_picture2(20, 63, 76); // (row, column)

  // Flags
//  Neomatrix_scroll_picture2(0,  13, 30,100); // (xoffset, row, column, delay)
//  Neomatrix_scroll_picture2(24, 13, 30, 100); // (xoffset, row, column, delay)
//  Neomatrix_scroll_picture2(8, 13, 30, 100); // (xoffset, row, column, delay)
//  Neomatrix_scroll_picture2(32, 13, 30, 100); // (xoffset, row, column, delay)
//  Neomatrix_scroll_picture2(16, 13, 30, 100); // (xoffset, row, column, delay)
  
  //Neomatrix_scroll_picture2(20, 30, 31); // (xoffset, row, column)
  //Neomatrix_scroll_picture2(30, 30, 31); // (xoffset, row, column)
 // Neomatrix_scroll_picture2(32, 13, 30); // (row, column)
}

// ============== Send Text =================

void Send_Text(String inputstr) {
  ArduinoOTA.handle();
  int randn = random(0,767);
  byte red = 200;
  byte green = 200;
  byte blue = 0;

  if (randn < 256) {
      red = 255 - randn;
      green = randn;
  } else if (randn < 511) {
      green = 511 - randn;
      blue = randn - 256;
  } else {
      red = 767 - randn;
      blue = randn - 512;
  }
  
  Neomatrix_scrolltext(inputstr ,red, green, blue);
}

// ============== Send Text Two words random =================

void Send_Text_two_words_random() {
  bool Toggle_Color;
  byte r1,g1,b1, r2,g2,b2;
  ArduinoOTA.handle();

  
  r1 = 205  ; r2 = 0;
  g1 = 0  ; g2 = 0;
  b1 = 0   ; b2 = 255;
  
  for (int x=0 ; x < ((Scroll_Text[0].length()+Scroll_Text[1].length())*CHAR_WIDTH+NCOLUMNS); x++) {
     if ( (x % (CHAR_WIDTH*8)) == 0 ) {
     //if ( (x % 3) == 0 ) {
        //Toggle_Color = !Toggle_Color;
        //if (Toggle_Color) {
        //  g1 = 60; g2 = 0;
        //} else {
        //  g1 = 0; g2=60;
        //}
        Neomatrix_random_color();
        r1 = red_random;
        g1 = green_random;
        b1 = blue_random;
        Neomatrix_random_color();
        r2 = red_random;
        g2 = green_random;
        b2 = blue_random;
     }
     
     matrix.fillScreen(0);

     Neomatrix_text(Scroll_Text[0], r1, g1, b1, -x+NCOLUMNS-1);
     Neomatrix_text(Scroll_Text[1], r2, g2, b2, -x+NCOLUMNS-1 + (Scroll_Text[0].length() + 3)*CHAR_WIDTH);
   
     matrix.show();
     delay(10);
  }
}


// ==================================================================================================

void Send_Text_two_words() {
  bool Toggle_Color;
  byte r1,g1,b1, r2,g2,b2;
  ArduinoOTA.handle();

  
  r1 = 205  ; r2 = 0;
  g1 = 0  ; g2 = 0;
  b1 = 0   ; b2 = 255;
  
  for (int x=0 ; x < ((Scroll_Text[0].length()+Scroll_Text[1].length())*CHAR_WIDTH+NCOLUMNS); x++) {
     matrix.fillScreen(0);
     Neomatrix_text(Scroll_Text[0], r1, g1, b1, -x+NCOLUMNS-1);
     Neomatrix_text(Scroll_Text[1], r2, g2, b2, -x+NCOLUMNS-1 + (Scroll_Text[0].length() + 3)*CHAR_WIDTH);
     matrix.show();
     delay(10);
  }
}

// ===================================================================================================

void Send_Text_Random() {
  byte r1,g1,b1, r2,g2,b2;
  ArduinoOTA.handle();
  
  for (int x=0 ; x < ((Scroll_Text[0].length()+Scroll_Text[1].length())*CHAR_WIDTH+NCOLUMNS); x++) {
     if ( (x % (CHAR_WIDTH*8)) == 0 ) {
        Neomatrix_random_color();
        r1 = red_random;
        g1 = green_random;
        b1 = blue_random;
        Neomatrix_random_color();
        r2 = red_random;
        g2 = green_random;
        b2 = blue_random;
     }
     
     matrix.fillScreen(0);

     Neomatrix_text(Scroll_Text[0], r1, g1, b1, -x+NCOLUMNS-1);
     Neomatrix_text(Scroll_Text[1], r2, g2, b2, -x+NCOLUMNS-1 + (Scroll_Text[0].length() + 3)*CHAR_WIDTH);
   
     matrix.show();
     delay(5);
  }
}



void Send_Text2(String inputstr) {
  ArduinoOTA.handle();

  Neomatrix_random_color();
  Neomatrix_scrolltext(inputstr ,red_random, green_random, blue_random);
}
