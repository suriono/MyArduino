#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

JSONVar myJSON;
WiFiServer server(8000);     // port 8000 for incoming wifi
WiFiClient wificlient;
const char* host = "OTA-MOWER_FRIENDLY";
unsigned long lasttime;

#define DEBUGGING
//#define LED_PIN     13

// ========================== Set up =======================

void setup() {
  //pinMode(LED_PIN, OUTPUT);
  Motor_Init();
  Serial.begin(57600);

  WiFi.softAP("FriendlyRobotics", "hello5239612");
  Serial.print("AP IP: "); Serial.println(WiFi.softAPIP());
  //WiFi.enableAP(false);
  //WiFi.mode(WIFI_STA);
 
  //wifi_begin();
 // WiFi.setAutoReconnect(true);
  //WiFi.persistent(true);
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

  

 // Motor_Forward(1000, 100);
  Motor_Stop();
 // Motor_Reverse(2000, 100);
 // Motor_Left(2000, 100);
 // Motor_Right(2000, 100);
// Motor_Mow(1000,100);
//  Motor_Mow(2000,100);

  Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());
}

// ========================== Loop =======================

void loop() {
  ArduinoOTA.handle();

  if (incoming_wifi()) {
     process_JSON();
     lasttime = millis();
  } else {
     if ( (millis() - lasttime) > 1000) { // stop no incoming wifi
        #ifdef DEBUGGING
           Serial.println("No incoming, stop the motor");
        #endif
        Motor_Stop();
        lasttime = millis();
     }
  }
}
