#include "EspMQTTClient.h"


#include <Arduino_JSON.h>
#include <ESP8266mDNS.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include <Wire.h>
//#include <ESP8266WiFi.h>

WiFiServer server(8000);     // port 8000 for incoming wifi
WiFiClient wificlient;

// === IMU =============
#define INTERVAL_IMU  2   // interval of reading IMU
MPU6050 mpu;                 // AD0 = LOW !!!!
int     Yaw;
#define INTERRUPT_PIN  13    // 13=D7, 2 = D4  connect to MPU Interrupt pin
bool    IMU_Ready = false;     // set true if DMP init was successful

// === MQTT ============
#define WIFI_SSID        "philip-EXT2"
#define WIFI_PASSWD      "fern5077266123"
#define MQTT_USER        "uz"
#define MQTT_PASSWD      ""
#define MQTT_IP          "192.168.11.234"

#define MQTT_TOPIC_YAW   "mower/imu/yaw"
#define MQTT_TOPIC_COUNT "mower/imu/count"

EspMQTTClient mqtt_client( WIFI_SSID, WIFI_PASSWD, MQTT_IP, MQTT_USER, MQTT_PASSWD, "moweranglemqtt",1883); // passwords are hidden for security
//EspMQTTClient mqttclient( WIFI_SSID, WIFI_PASSWD, MQTT_IP, MQTT_USER, MQTT_PASSWD, "gpsmqtt",1883);
JSONVar myJSON;

// ======================= SETUP =======================================

void setup() {
  delay(5000);
  Serial.begin(57600);
  WiFi.enableAP(false);
  WiFi.mode(WIFI_STA);

  mqtt_client.enableMQTTPersistence();   // MQTT client

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
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
   mqtt_client.loop();
   static unsigned long last_valid_json;
  
   if (IMU_Ready) {
      
      IMU_Yaw();                   // get IMU Yaw
      
   
      if (incoming_wifi()) {
         process_JSON();
  //       last_valid_json = millis();
      }
   }
   
 // if ( (millis() - last_valid_json) > 1000) { // stop when no signal
    //  last_valid_json = millis();
    //  Serial.println("QMMMMM");
    //  MQTT_publish_string("hello");
  //}

}
