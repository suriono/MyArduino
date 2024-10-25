#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>

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
  Serial.begin(115200);

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
  
}

// =============== Loop ======================

void loop() {

//  Send_Text("O say, can you see");
//  Send_Text("By the dawn's early light");
//  Send_Text("What so proudly we hail");
//  Send_Text("At the twilight's last gleaming");
//
//  Send_Text("Whose broad stripes and bright stars");
//  Send_Text("Through the perilous fight");
//  Send_Text("O'er the ramparts we watch");
//  Send_Text("Were so gallantly streaming");
//
//  Send_Text("And the rocket's red glare");
//  Send_Text("The bombs bursting in air");
//  Send_Text("Gave proof through the night");
//  Send_Text("That our flag was still there");
//
//  Send_Text("O say, does that star-spangled banner yet wave");
//  Send_Text("O'er the land of the free");
//  Send_Text("And the home of the brave");
 
  //Send_Text();

  //Neomatrix_scrolltext_random_eachstep("Merry Christmas");

  Send_Text("13934 Fallbrook Way");
  
  Neomatrix_scroll_picture2(0, 40, 35); // (row, column)
  Neomatrix_scroll_picture2(29, 40, 35); // (row, column)
 // Neomatrix_scroll_picture2(32, 13, 30); // (row, column)
}

// ============== Send Text =================

void Send_Text(String inputstr) {

  ArduinoOTA.handle();

  int randn = random(0,767);
  byte red = 0;
  byte green = 0;
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
