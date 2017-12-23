/* NodeMCU pins mapping to Arduino
D0   = 16;    D1   = 5;     D2   = 4;
D3   = 0;     D4   = 2;     D5   = 14;    D6   = 12;
D7   = 13;    D8   = 15;    D9   = 3;     D10  = 1;
*/

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoOTA.h>
#include <WiFiUdp.h>       // used for network Epoch
#include "NewPing.h"

// Set these to run example.
#define FIREBASE_HOST "xxx"
#define FIREBASE_AUTH "xxx"
#define WIFI_SSID "xxx"
#define WIFI_PASSWORD "xxx"


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
#define CLOUD_SEND_INTERVAL 3600000   // in milliseconds should be around 120000 (2 minutes)
#define DISTANCE_MEAS_INTERVAL 10000   // in msec minimum measurement delay
#define DEBUG_INTERVAL 60000          // Firebase debug update

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

unsigned long time_program_start = millis();
long last_resettime;
unsigned long time_firebase_update = millis()-70000;
unsigned long time_this_program_start = millis();
int last_distance;

#define DEBUG

// ============= Firebase =============================

// --------------------------------------------------
bool Firebase_getPushButtonRemote() {  // whether to push the wall remote button to open/close
  int wall_pin = Firebase.getString("garagedoor/pushButtonWallRemote").toInt();
  digitalWrite(RELAY_OPENER, LOW);
  
  //if (Firebase.failed()) {
  //  Firebase.setString("garagedoor/pushButtonWallRemote", "0");
  //  return false;
  //}
  Firebase.setString("garagedoor/pushButtonWallRemote", "1");
  if (Firebase.failed()) {
    return false;
  }
  int reset_wall_pin = Firebase.getString("garagedoor/pushButtonWallRemote").toInt();
  if ( reset_wall_pin == 1) { // make sure it has been reset so it does not open/close continuously
    if (wall_pin == 5239612) {  // pin number to open the garage
      digitalWrite(RELAY_OPENER, HIGH);
      delay(500);
      digitalWrite(RELAY_OPENER, LOW);
    } else {
      return false;
    }
  } else {
    return false;
  }
  return true;
}
// ---------------------------------------------------
long Firebase_getResetTime() {
  long resettime = Firebase.getString("garagedoor/resetTime").toInt();

  //Serial.print("reset time = "); Serial.println(Firebase.getString("garagedoor/resetTime"));
  if (Firebase.failed()) {
      Firebase_fail();
      return 0;
  }
  return resettime;
}
// ------------------------------------------------
bool Firebase_Send_Distance(int new_distance) {
  // static float last_resettime;
  long resettime = Firebase_getResetTime();
  unsigned long server_epoch = get_Server_Time();

  
  if ( resettime <= 0 || server_epoch <= 0 ) return false;
     
  unsigned long new_time = server_epoch - resettime;

  if ( new_time > 10000000 ) return false; // if something wrong
      
  String sendstr = "dist:" + String(new_distance) + "&time:" + String(new_time);
  Firebase.pushString("garagedoor/data/", sendstr);
  //Serial.println("==== Updating Firebase");
  if (Firebase.failed()) {
     Firebase_fail();
     return false;
  }
 
  time_firebase_update = millis();
  return true;
}

// ------------------------------------------------------
void Firebase_debug(String debug_msg) {
  Firebase.setString("garagedoor/debug/", debug_msg);
}

// -------------------------------------------------------
void Firebase_fail() {
   Serial.print("==== failed: ");
   Serial.println(Firebase.error());  

   // resetFunc(); //call reset 
   // Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
void(* resetFunc) (void) = 0;//declare reset function at address 0

// ============= End of Firebase ================================

// ============= Take ultra sonic distance in cm =================
int Get_Sonic_Distance() {
  for (int nn = 0 ; nn < 10; nn++) {
    int tmp_distance = int(sonar.ping_median(11)/50); // in cm
    if (tmp_distance > 0) {
      return tmp_distance;
    }
    delay(100);
  }

  return 400;   // assuming it's close when it's too far away
}

// ================ NTP request used for Epoch ===============

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress& address)
{
  //Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

// ====================== get time from server ===============
unsigned long get_Server_Time() {
  
      // getting epoch time from a server
      WiFi.hostByName(ntpServerName, timeServerIP); 
      sendNTPpacket(timeServerIP); // send an NTP packet to a time server
      delay(2000);
      int cb = udp.parsePacket();
      if (!cb) {
         Serial.println("no packet yet");
         return 0;
      }
      else {
        //Serial.println(cb);
        // We've received a packet, read the data from it
        udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
        //the timestamp starts at byte 40 of the received packet and is four bytes,
        // or two words, long. First, esxtract the two words:
        unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
        unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
        // combine the four bytes (two words) into a long integer
        // this is NTP time (seconds since Jan 1 1900):
        unsigned long secsSince1900 = highWord << 16 | lowWord;
        //Serial.print("Seconds since Jan 1 1900 = " );
        //Serial.println(secsSince1900);
        //Serial.print("Unix time = ");
        // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
        const unsigned long seventyYears = 2208988800UL;
        // subtract seventy years:
        unsigned long epoch = secsSince1900 - seventyYears;
        // print Unix time:
        //Serial.println(epoch);
        return epoch;

      }
}

// ====================== setup ===============================

void setup() {
  Serial.begin(9600);

   // connect to wifi.
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

  // OTA setups
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);
  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("esp8266-garage-door");
  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();

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
  //static int last_distance;

  ArduinoOTA.handle();     // if any OTA update
  
  int new_distance = Get_Sonic_Distance();

  Firebase_getPushButtonRemote(); // check whether to open/close garage
  //digitalWrite(RELAY_OPENER, HIGH);
  //delay(500);
  //digitalWrite(RELAY_OPENER, LOW);
  
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

        Firebase_debug("Dist@" + String(new_distance) + "^Epoch@" + String(get_Server_Time()));
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
  delay(DISTANCE_MEAS_INTERVAL);
}


