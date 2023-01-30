#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <SabertoothSimplified.h>
#include <SoftwareSerial.h>

SoftwareSerial SerialRobot(5 , 4);     // RX, TX
SabertoothSimplified ST(SerialRobot);

JSONVar myJSON;
WiFiServer server(8000);         // port 8000 for incoming wifi
WiFiClient wificlient;
const char* host = "OTA-MOWER_ROBOT";
//unsigned long last_motor_run;

// #define DEBUG 

void setup() {
  SerialRobot.begin(38400);
  #ifdef DEBUG
    Serial.begin(57600);
  #endif
  WiFi.enableAP(false);
  WiFi.mode(WIFI_STA);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  motorStop();

  wifi_begin();

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
  server.begin();
 // Serial.println("Ready");

  motorStop();
//  ST.motor(1, 100);
  delay(1000);
//  ST.motor(1, 0);
  motorRun(40, 0, 2000);
  delay(1000);
  motorRun(40,180, 2000);
  motorStop();
}

void loop() {
  static unsigned long last_valid_json;
  // put your main code here, to run repeatedly:
  //digitalWrite(LED_BUILTIN,HIGH);
  //delay(500);
  //digitalWrite(LED_BUILTIN,LOW);
  //delay(1200);
  ArduinoOTA.handle();

  if (incoming_wifi()) {
    process_JSON();
    last_valid_json = millis();
  } else if ( (millis() - last_valid_json) > 1000) { // stop when no signal
    motorStop();
    last_valid_json = millis();
  }
}
