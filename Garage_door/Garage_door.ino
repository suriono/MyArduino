/* NodeMCU pins mapping to Arduino
D0   = 16;    D1   = 5;     D2   = 4;
D3   = 0;     D4   = 2;     D5   = 14;    D6   = 12;
D7   = 13;    D8   = 15;    D9   = 3;     D10  = 1;
*/

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
// #include <ArduinoOTA.h>
#include <WiFiUdp.h>       // used for network Epoch
#include "NewPing.h"

// Set these to run example.
#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""
#define WIFI_SSID "philip"
#define WIFI_PASSWORD ""
#define WALL_PIN 22


// Epoch time server
unsigned int localUDPPort = 2390;      // local port to listen for UDP packets
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
WiFiUDP udp;

// === garage door opener relay
#define RELAY_OPENER 14  // D5 of NodeMCU

// ======= Below Ultra Sonic sensor =========
#define TRIGGER_PIN  5  // D1 of NodeMCU
#define ECHO_PIN     4  // D2 of NodeMCU
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define CLOUD_SEND_INTERVAL  3600000   // in milliseconds should be around 120000 (2 minutes)
#define DISTANCE_MEAS_INTERVAL 7000   // in msec minimum measurement delay
#define DEBUG_INTERVAL 30000          // Firebase debug update

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

unsigned long time_program_start = millis();
long last_resettime;
unsigned long time_firebase_update = millis()-70000;
unsigned long time_this_program_start = millis();
int last_distance;
unsigned long last_time_distance;

#define DEBUG




// ====================== setup ===============================

void setup() {
  Serial.begin(9600);

  // connect to wifi.
  //ESP.eraseConfig(); delay(1000);
  WiFi.enableAP(false);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  // OTA_Setup();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  Serial.println("Starting UDP");
  udp.begin(localUDPPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());
  
  last_resettime = Firebase_getResetTime();

  pinMode(RELAY_OPENER, OUTPUT);
  digitalWrite(RELAY_OPENER, LOW);
}


// ====================== LOOP =========================

void loop() {
  static long delta_last_time_distance;
  // ArduinoOTA.handle();     // if any OTA update
  
  int new_distance = Get_Sonic_Distance();
  last_time_distance = millis();
  Serial.print("Garage door distance: "); Serial.print(new_distance);
  Serial.print(", time: "); Serial.println(millis());

  Firebase_getPushButtonRemote(); // check whether to open/close garage
  
  if ( ( (millis() - time_firebase_update) > CLOUD_SEND_INTERVAL || 
         ( last_distance < 80 && new_distance > 100) ||
         ( last_distance > 100 && new_distance < 80)
        ) && new_distance > 0) {  
 
      if (Firebase_Send_Distance(new_distance) ) {
        //time_firebase_update = millis();
        last_distance = new_distance;
      }
  } else {
      
      static unsigned long last_debug_time;
      if ((millis() - last_debug_time) > DEBUG_INTERVAL) {

        unsigned long tmp_time = get_Server_Time();
  
        if (tmp_time < 1) { // try again
          tmp_time = delta_last_time_distance + last_time_distance / 1000;
          Serial.println("Zero Debug Server Epoch =====");
        } else {
          delta_last_time_distance = tmp_time - last_time_distance / 1000;
        }
        Serial.print("Debug epoch: "); Serial.println(tmp_time);

        Firebase_debug("Dist@" + String(new_distance) + "^Epoch@" + String(tmp_time));
        last_debug_time = millis();

        long resettime = Firebase_getResetTime();
        if (resettime > last_resettime) {
        
          Firebase.remove("garagedoor/data/");
          if (Firebase.success()) {
            last_resettime = resettime;
        
            if (Firebase_Send_Distance(new_distance) ) {
              //time_firebase_update = millis();
              last_distance = new_distance;
            }
            delay(2000);
          }
        }
    }
  }

  while ( (millis() - last_time_distance) < DISTANCE_MEAS_INTERVAL) {
    delay(1000);
  }
}


