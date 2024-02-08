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
String Scroll_Text[2] = {"Happy Chinese New Year", "The year of the Dragon"};
void loop() {

  //Send_Text("Merry Christmas");
  //Send_Text("Joy to the world"); 
  //Send_Text("Let us receive the King");
  Send_Text();

  //Neomatrix_scrolltext_random_eachstep("Merry Christmas");
  
  Neomatrix_scroll_picture2(0, 45, 64);
  
}

// ============== Send Text =================

void Send_Text() {
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
