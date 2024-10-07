#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
//#include <ArduinoWiFiServer.h>
//#include "ESPAsyncTCP.h"
//#include "SyncClient.h"


WiFiClient wificlient;
WiFiServer server(8000);     // port 8000 for incoming wifi
//ArduinoWiFiServer server(8000);

void setup() {
  Serial.begin(57600);
 // Serial.swap(); // (Rx2,D7) and 15(Tx2,D8)
  delay(1000); // Wait for a second
  Serial1.begin(19200);
  
  WiFi.softAP("FriendlyRoboticsGPS", "helloworld");
  Serial.print("AP IP: "); Serial.println(WiFi.softAPIP());
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  //wificlient.setTimeout(1000);

  
  server.begin();
}

void loop() {
  if (incoming_wifi()) {
 
 // } else {
  //  Serial.println("no");
  //  delay(100);
  }
}
