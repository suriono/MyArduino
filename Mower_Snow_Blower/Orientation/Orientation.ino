#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>
#include <ESP8266mDNS.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

JSONVar myJSON;
WiFiServer server(8000);     // port 8000 for incoming wifi
WiFiClient wificlient;

#define INTERVAL_IMU  2   // interval of reading IMU
MPU6050 mpu;                 // AD0 = LOW !!!!
int     Yaw;
#define INTERRUPT_PIN  2    // connect to MPU Interrupt pin
bool    IMU_Ready = false;     // set true if DMP init was successful

void setup() {
  delay(5000);
  Serial.begin(57600);
  WiFi.enableAP(false);
  WiFi.mode(WIFI_STA);

  Wire.begin();    // for IMU
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
  IPAddress ip(192, 168, 11, 203); 
  IPAddress gateway(192, 168, 11, 1); // set gateway to match your network
  IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your
  WiFi.config(ip, gateway, subnet);
  wifi_begin();                       // contain passowrd, hidden for security
  wificlient.setTimeout(200);
  server.begin();
  delay(1000);

  IMU_begin();
}

void loop() {
   static unsigned long last_valid_json;
  
   if (IMU_Ready) {
      IMU_Yaw();                   // get IMU Yaw
   
      if (incoming_wifi()) {
         process_JSON();
  //       last_valid_json = millis();
      }
   }
   
  // if ( (millis() - last_valid_json) > 1000) { // stop when no signal
  //    last_valid_json = millis();
  // }

}
