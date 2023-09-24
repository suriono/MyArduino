/* NodeMCU pins mapping to Arduino
D0   = 16;    D1   = 5;     D2   = 4;
D3   = 0;     D4   = 2;     D5   = 14;    D6   = 12;
D7   = 13;    D8   = 15;    D9   = 3;     D10  = 1;
*/

#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
// Provide the token generation process info.
#include <addons/TokenHelper.h>
// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>
// #include <ArduinoOTA.h>
#include <WiFiUdp.h>       // used for network Epoch
#include <SoftwareSerial.h>

SoftwareSerial SerialSonar(5, 4); // RX TX only RX 14 needed

WiFiUDP udp;
//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

// === garage door opener relay
#define RELAY_OPENER 14                // D5 of NodeMCU
#define CLOUD_SEND_INTERVAL  3600000   // in milliseconds should be around 120000 (2 minutes)
#define DEBUG_INTERVAL 60002           // Firebase debug update

unsigned long time_program_start = millis();
long last_resettime = 0;
unsigned long last_server_epoch, last_server_epoch_elapsed;
unsigned long time_firebase_update; // = millis()-70000;
unsigned long time_this_program_start = millis();
int last_distance;

#define DEBUG

int Get_Sonic_Distance(); unsigned long get_Server_Time(); void Udp_begin();
void Firebase_debug(String); void WiFi_begin(); void Firebase_begin(); 

// ====================== setup ===============================

void setup() {
  Serial.begin(57600);
  SerialSonar.begin(9600);

  // connect to wifi.
  //ESP.eraseConfig(); delay(1000);
  WiFi.setOutputPower(20.0); 
  WiFi.enableAP(false);
  WiFi_begin();
  
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  // OTA_Setup();
  Firebase_begin();
  //Firebase_getResetTime(0,"Initial");

  Udp_begin();

  pinMode(RELAY_OPENER, OUTPUT);
  digitalWrite(RELAY_OPENER, LOW);
}

// ====================== LOOP =========================

void loop() { 
  static unsigned long last_debug_time;
  // ArduinoOTA.handle();     // if any OTA update
  
  int new_distance = Get_Sonic_Distance();
  Serial.print("Read distance : "); Serial.println(new_distance);
  if (new_distance == 400) new_distance = last_distance;

  Serial.print(new_distance); Serial.print(",");
  
  //Firebase_getPushButtonRemote(); // check whether to open/close garage
  
  if ( ( (millis() - time_firebase_update) > CLOUD_SEND_INTERVAL || 
         ( last_distance < 80 && new_distance > 100) ||
         ( last_distance > 100 && new_distance < 80)
        ) && new_distance > 0) {  // big distance change
 
        Firebase_Send_Distance(new_distance);
        last_distance = new_distance;
        last_debug_time = millis(); // so the debug doesn't go too often
      
  }  else if ((millis() - last_debug_time) > DEBUG_INTERVAL) {
        
        unsigned long tmp_time = get_Server_Time();
        if (tmp_time < 199) { // no data packet for server time
          tmp_time = last_server_epoch + (millis() - last_server_epoch_elapsed)/1000; // 199 just arbitrary 
        }
        Firebase_debug("Dist@" + String(new_distance) + "^Epoch@" + String(tmp_time));
        last_debug_time = millis();

        //long resettime = last_resettime; // to check any reset time change
        //Serial.print("debug resettime: "); Serial.println(resettime);
        //Firebase_getResetTime(new_distance, "^Dist@" + String(new_distance) + "^Epoch@" + String(tmp_time));
        //Serial.print("debug last_resettime: "); Serial.println(last_resettime);
        //if (resettime < last_resettime) {
         //   Firebase.remove("garagedoor/data/"); // remove data
         //   Firebase_Send_Distance(new_distance);
       // }
  }
}
