#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

JSONVar myJSON;
WiFiServer server(8000);     // port 8000 for incoming wifi
WiFiClient wificlient;
const char* host = "OTA-MOWER_FRIENDLY";

#define DEBUGGING

// ========================== Set up =======================

void setup() {
  Motor_Init();
  Serial.begin(57600);
  WiFi.enableAP(false);
  WiFi.mode(WIFI_STA);

  
 
  wifi_begin();
  wificlient.setTimeout(200);

  ArduinoOTA.setHostname(host);
  ArduinoOTA.onStart([]() { // switch off all the PWMs during upgrade
     });
  ArduinoOTA.onEnd([]() { // do a fancy thing with our board led at end
  });
  ArduinoOTA.onError([](ota_error_t error) {
    (void)error;
    ESP.restart();
  });
  ArduinoOTA.begin();
  server.begin();

  Motor_Forward(2000, 100);
  Motor_Reverse(2000, 100);
  Motor_Left(2000, 100);
  Motor_Right(2000, 100);
  Motor_Mow(1000,100);
  Motor_Mow(2000,100);
}

// ========================== Loop =======================

void loop() {
  ArduinoOTA.handle();
  // put your main code here, to run repeatedly:

}
