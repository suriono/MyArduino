#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <SabertoothSimplified.h>
#include <SoftwareSerial.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

#define DEBUGGING 

SoftwareSerial SerialRobot(D6 , D7);     // RX, TX
SabertoothSimplified ST(SerialRobot);

JSONVar myJSON;
WiFiServer server(8000);     // port 8000 for incoming wifi
WiFiClient wificlient;
const char* host = "OTA-MOWER_ROBOT";

#define INTERVAL_IMU  100   // interval of reading IMU
MPU6050 mpu;                 // AD0 = LOW !!!!
int     Yaw;
#define INTERRUPT_PIN  2    // use pin 2 on Arduino Uno & most boards
bool IMU_Ready = false;     // set true if DMP init was successful

// ========================== Set up =======================

void setup() {
  delay(5000);
  SerialRobot.begin(38400);
  Serial.begin(57600);
  //WiFi.enableAP(false);
  //WiFi.mode(WIFI_STA);

  Wire.begin();    // for IMU
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties

  motorStop();

  //wifi_begin();
  WiFi.softAP("PlowerRobotics", "hello12345");
  Serial.print("AP IP: "); Serial.println(WiFi.softAPIP());
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

  delay(1000);
  //motorRun(40,180, 2000);
  motorStop();

  IMU_begin();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
   static unsigned long last_valid_json;
  
   ArduinoOTA.handle();

   if (IMU_Ready) {
      IMU_Yaw();                   // get IMU Yaw
   
      if (incoming_wifi()) {
         process_JSON();
         last_valid_json = millis();
      }
   }
   
   if ( (millis() - last_valid_json) > 1000) { // stop when no signal
      motorStop();
      last_valid_json = millis();
   }
}
