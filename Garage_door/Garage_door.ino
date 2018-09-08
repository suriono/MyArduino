/* NodeMCU pins mapping to Arduino
D0   = 16;    D1   = 5;     D2   = 4;
D3   = 0;     D4   = 2;     D5   = 14;    D6   = 12;
D7   = 13;    D8   = 15;    D9   = 3;     D10  = 1;
*/

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
// #include <ArduinoOTA.h>
#include <WiFiUdp.h>       // used for network Epoch
#include <SoftwareSerial.h>

SoftwareSerial SerialSonar(5, 4); // RX TX only transmit 13 needed

WiFiUDP udp;

// === garage door opener relay
#define RELAY_OPENER 14                // D5 of NodeMCU
#define CLOUD_SEND_INTERVAL  3600000   // in milliseconds should be around 120000 (2 minutes)
#define DEBUG_INTERVAL 60002           // Firebase debug update

unsigned long time_program_start = millis();
long last_resettime;
unsigned long time_firebase_update = millis()-70000;
unsigned long time_this_program_start = millis();
int last_distance;
//unsigned long last_time_distance;
long delta_reset_millis = 0;          // the difference between millis() and reset time

#define DEBUG

// ====================== setup ===============================
//void WiFi_begin(); void Firebase_begin();
void setup() {
  Serial.begin(9600);
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
  Firebase_getResetTime();

  Udp_begin();

  pinMode(RELAY_OPENER, OUTPUT);
  digitalWrite(RELAY_OPENER, LOW);
}


// ====================== LOOP =========================

void loop() { 
  static unsigned long last_debug_time;
  // ArduinoOTA.handle();     // if any OTA update
  
  int new_distance = Get_Sonic_Distance();
  if (new_distance == 400) new_distance = last_distance;
  //last_time_distance = millis();
  Serial.print("Garage door distance: "); Serial.print(new_distance);
  //Serial.print(", time: "); Serial.println(millis());
  
  //Firebase_getPushButtonRemote(); // check whether to open/close garage
  
  if ( ( (millis() - time_firebase_update) > CLOUD_SEND_INTERVAL || 
         ( last_distance < 80 && new_distance > 100) ||
         ( last_distance > 100 && new_distance < 80)
        ) && new_distance > 0) {  // big distance change
 
      //if (Firebase_Send_Distance(new_distance) ) {
        Firebase_Send_Distance(new_distance);
        last_distance = new_distance;
        last_debug_time = millis(); // so the debug doesn't go too often
      //}
  }  else {
      if ((millis() - last_debug_time) > DEBUG_INTERVAL) {
        
        Firebase_getResetTime();
        
        unsigned long tmp_time = get_Server_Time();
        Firebase_debug("Dist@" + String(new_distance) + "^Epoch@" + String(tmp_time));
        last_debug_time = millis();

      }
  }
        /*
        //Serial.print("Debug epoch: "); Serial.println(tmp_time);
        //Serial.print("Delta time: "); Serial.println(delta_reset_millis);

        Firebase_debug("Dist@" + String(new_distance) + "^Epoch@" + String(tmp_time));
        last_debug_time = millis();

        long resettime = Firebase_getResetTime();
        if (resettime > last_resettime) {
        
          Firebase.remove("garagedoor/data/");
          if (Firebase.success()) {
            last_resettime = resettime;
        
            if (Firebase_Send_Distance(new_distance) ) {
              last_distance = new_distance;
              last_debug_time = millis(); // so the debug doesn't go too often
            }
            delay(2000);
          }
        }
    }
  }

  //while ( (millis() - last_time_distance) < DISTANCE_MEAS_INTERVAL) {
  //  delay(1000);
  //}
  */
}


