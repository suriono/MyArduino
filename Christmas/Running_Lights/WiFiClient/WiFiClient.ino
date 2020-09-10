#include <ESP8266WiFi.h>
#include <WiFiClient.h>

WiFiServer server(80);
WiFiClient client;

// ===================================

void setup() {
  Serial.begin(57600);
  Serial1.begin(57600);
  //Serial1.setTimeout(3000);

  Wifi_Begin();
  Wifi_init();
}

// ====================================

void loop() {
  client = server.available();
  if (client) {
    incoming_client();
  }
}
