#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>
#include "password.h"
#include "certs.h"

X509List cert(cert_DigiCert_Global_Root_CA);

#define NEOPIXEL_PIN    0  // D3 for NodeMCU
#define NCOLUMNS        64 // number of pixel columns
#define CHAR_WIDTH      6  // font width

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(NCOLUMNS, 8, NEOPIXEL_PIN,
  NEO_MATRIX_TOP  + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + 
  NEO_MATRIX_ZIGZAG, NEO_GRB + NEO_KHZ800);

String Alert_Message;

void setup() {
  Serial.begin(57600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  Neopixel_Initial();
  Alert_Message = get_Weather();
  
}

void loop() {
  Neomatrix_scrolltext(Alert_Message, 255,0,255);
  // put your main code here, to run repeatedly:

}
