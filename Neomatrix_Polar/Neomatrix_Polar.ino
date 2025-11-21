#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>
#include "password.h"
#include "certs.h"

X509List cert(cert_DigiCert_Global_Root_CA);

#define NEOPIXEL_PIN    12  // 0=D3 12=D6 for NodeMCU
#define NCOLUMNS        32  // number of pixel columns
#define CHAR_WIDTH      6   // font width
#define RPWM_PIN        13   // D2=4
#define LPWM_PIN        15   // D1=5
#define FADE_DELAY      10

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(NCOLUMNS, 8, NEOPIXEL_PIN,
  NEO_MATRIX_TOP  + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + 
  NEO_MATRIX_ZIGZAG, NEO_GRB + NEO_KHZ800);

String Alert_Message;
//void Neopixel_Initial();
//void Neomatrix_scrolltext(String instr, byte R, byte G, byte B);

void setup() {
  Serial.begin(57600);
  pinMode(RPWM_PIN,OUTPUT); pinMode(LPWM_PIN,OUTPUT);
 // WiFi.begin(WIFI_SSID, WIFI_PASSWD);
//  while (WiFi.status() != WL_CONNECTED) {
 //   delay(500);
 //   Serial.print(".");
 // }
 // Serial.println("");
 // Serial.println("WiFi connected");
 // Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
  
  Neopixel_Initial();
  analogWrite(LPWM_PIN,0); analogWrite(RPWM_PIN,0);
  //Alert_Message = get_Weather();
  
}

void loop() {
  Send_Text("Happy ThanksGiving");
  Send_Text("What are you thankful for?");
  Send_Text("Brought to you by Polar Semiconductor teams");
  
  Neomatrix_scroll_picture2(0,  35, 32,50); // (xoffset, row, column, delay)
 // Neomatrix_scrolltext("Hello Uz", 50, 0, 0);
  //Neomatrix_scroll_picture_random_color(0 ,  26, 32,100); // (xoffset, row, column, delay)
  
 // Neomatrix_horizontal_scroll_picture_random_color(0 ,  8, 7,100); // (xoffset, row, column, delay)
 // Neomatrix_scrolltext(Alert_Message, 255,0,255);
  // put your main code here, to run repeatedly:

  analogWrite(LPWM_PIN,0);
  //Serial.println("EN High");
 // for(int i=0;i<156;i++){
  //  analogWrite(RPWM_PIN,i);
 //   delay(FADE_DELAY);
 // }
  //delay(200);
 // Serial.println();
 // for(int i=155;i>0;i--){
 //   analogWrite(RPWM_PIN,i);
 //   delay(FADE_DELAY);
 // }
}

// ============== Send Text =================

void Send_Text(String inputstr) {
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
