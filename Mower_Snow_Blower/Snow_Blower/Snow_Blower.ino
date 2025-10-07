#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>

JSONVar myJSON;
WiFiServer server(8000);     // port 8000 for incoming wifi
WiFiClient wificlient;

#define PIN_SNOWBLOWER            4     // 4 = D2

void setup() {
  Serial.begin(57600);
  pinMode(PIN_SNOWBLOWER, OUTPUT);    // D2 = GPIO4

  WiFi.enableAP(false);
  WiFi.mode(WIFI_STA);
  IPAddress ip(192, 168, 11, 202); 
  IPAddress gateway(192, 168, 11, 1); // set gateway to match your network
  IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your
  WiFi.config(ip, gateway, subnet);
  wifi_begin();

  server.begin();
}

void loop() {

  if ( incoming_wifi()) {
    process_JSON();
  }
}
