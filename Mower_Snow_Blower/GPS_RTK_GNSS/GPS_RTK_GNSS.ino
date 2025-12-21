#include "EspMQTTClient.h"
#include <Arduino_JSON.h>
#include <ESP8266mDNS.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h> //http://librarymanager/All#SparkFun_u-blox_GNSS
#include <Wire.h>  
#include "password.h"

#define myWire Wire // I2C communication to GPS

//WiFiServer server(8000);
//WiFiClient wificlient;

long lastTime = 0; //Simple local timer. Limits amount if I2C traffic to u-blox module.
SFE_UBLOX_GNSS myGNSS;
JSONVar myJSON;

// === MQTT ============

#define MQTT_IP          "192.168.11.234"
#define MQTT_TOPIC_GPS   "mower/gps"
//#define MQTT_TOPIC_LAT   "mower/gps/lat"
//#define MQTT_TOPIC_LON   "mower/gps/lon"
//#define MQTT_TOPIC_ACC   "mower/gps/acc"            // accuracy in mm
//#define MQTT_TOPIC_COUNT "mower/gps/count"
EspMQTTClient mqtt_client( WIFI_SSID, WIFI_PASSWD, MQTT_IP, MQTT_USER, MQTT_PASSWD, "mowergpsmqtt",1883); // passwords are hidden for security
int32_t latitude, longitude, accuracy;
int8_t latitudeHp, longitudeHp;
double d_lat, d_lon;
float f_accuracy;

// ========================================= SET UP ==================================

void setup()
{
  Serial.begin(57600);
  while (!Serial); //Wait for user to open terminal
  myWire.begin();
  mqtt_client.enableMQTTPersistence();   // MQTT client

  WiFi.enableAP(false);
  WiFi.mode(WIFI_STA);
  IPAddress ip(192, 168, 11, 201); 
  IPAddress gateway(192, 168, 11, 1); // set gateway to match your network
  IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your
  WiFi.config(ip, gateway, subnet);
  wifi_begin();
 // server.begin();
  
  //myGNSS.enableDebugging(Serial); // Uncomment this line to enable debug messages

  if (myGNSS.begin(myWire) == false) //Connect to the u-blox module using Wire port
  {
    Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1)
      ;
  }

  // Check that this platform supports 64-bit (8 byte) double
  if (sizeof(double) < 8)
  {
    Serial.println(F("Warning! Your platform does not support 64-bit double."));
    Serial.println(F("The latitude and longitude will be inaccurate."));
  }

  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)

  myGNSS.setNavigationFrequency(5); //Set output to 20 times a second

  byte rate = myGNSS.getNavigationFrequency(); //Get the update rate of this module
  Serial.print("Current update rate: ");
  Serial.println(rate);

  //myGNSS.saveConfiguration(); //Save the current settings to flash and BBR
}

// =========================== LOOP ================================

void loop() {
  mqtt_client.loop();
  
 // static long last_gps = millis();
  //Query module only every second.
  //The module only responds when a new position is available.
 // if (incoming_wifi()){
 //       json_Process() ;
 // } else {
    //Serial.println( (millis() - last_gps));
   // last_gps = millis();
    get_Coordinates(); 
  //}
  //if ( Serial.available() ) {
  //  Serial.println(Serial.read());
  //}
  
}
