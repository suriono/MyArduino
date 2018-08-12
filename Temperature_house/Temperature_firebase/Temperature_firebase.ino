/* NodeMCU pins mapping to Arduino
D0   = 16;    D1   = 5;     D2   = 4;
D3   = 0;     D4   = 2;     D5   = 14;    D6   = 12;
D7   = 13;    D8   = 15;    D9   = 3;     D10  = 1;
*/

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <WiFiUdp.h>       // used for network Epoch
#include <SoftwareSerial.h>

SoftwareSerial TempSerial(13, 0); // D7,  RX, TX for read temp

// Set these to run example.
<<<<<<< HEAD
#define FIREBASE_HOST "xxx"
#define FIREBASE_AUTH ""
#define WIFI_SSID "philip"
#define WIFI_PASSWORD "xxx"
=======
//#define FIREBASE_HOST "xxx"
//#define FIREBASE_AUTH "xxx"
//#define WIFI_SSID "xxx"
//#define WIFI_PASSWORD "xxx"
>>>>>>> parent of 13c1541... change the software serial 7->9


// Epoch time server
unsigned int localUDPPort = 2390;      // local port to listen for UDP packets
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
WiFiUDP udp;

//#define CLOUD_SEND_INTERVAL 15000  // in milliseconds should be around 120000 (2 minutes)
#define DEBUG_INTERVAL 60000          // Firebase debug update

unsigned long time_program_start = millis();
long last_resettime;
unsigned long time_firebase_update = millis()-70000;
unsigned long time_this_program_start = millis();
String TempInput = "";

#define DEBUG


// ========================== Setup =========================

void setup() {
  Serial.begin(9600);
  TempSerial.begin(9600);    // Temperature serial

<<<<<<< HEAD
  // connect to wifi.
  ESP.eraseConfig(); delay(1000);
  WiFi.enableAP(false);
  WiFi.mode(WIFI_STA);
=======
   // connect to wifi.
  WiFi.enableAP(false);
>>>>>>> parent of 13c1541... change the software serial 7->9
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(". check the wifi ssid and password");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  Serial.println("Starting UDP");
  udp.begin(localUDPPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());

  last_resettime = Firebase_getResetTime();
  
}

// ========================== Loop ==========================

void loop() {
  static float new_temperature;
  static boolean isTempAvailable = false;

  if (read_Temperature()) {
     Serial.print("Serial input:");
     Serial.println(TempInput);
     Serial.println(TempInput.substring(6,TempInput.indexOf('<')));
     //new_temperature = TempInput.substring(6,TempInput.indexOf('<')).toInt();
     new_temperature = TempInput.substring(6,TempInput.indexOf('<')).toFloat();
     Firebase_Send_Temperature(new_temperature);
  

  //if ( (millis() - time_firebase_update) > CLOUD_SEND_INTERVAL) {  
  //  Firebase_Send_Temperature(new_temperature++);
  } else {
    static unsigned long last_debug_time;
    if ((millis() - last_debug_time) > DEBUG_INTERVAL) {

      // for testing purpose:
      //Firebase_Send_Temperature(27);
      
      Firebase_debug("Temp@" + String(new_temperature) + "^Epoch@" + String(get_Server_Time()));
      last_debug_time = millis();

      long resettime = Firebase_getResetTime();
      if (resettime > last_resettime) { 
        Firebase.remove("fireplace/data/");
        if (Firebase.success()) {
          last_resettime = resettime;
          Firebase_Send_Temperature(new_temperature);
          delay(2000);
        }
      }
      delay(1000);
    }
    delay(1000);
  }
  delay(1000);
}

// ============= Read Temperature =====================

boolean read_Temperature() {
  char sread;
  static boolean ismySerialData = false;
  //static String YawInput;
  
  while (TempSerial.available()) {
     sread = TempSerial.read();
     //Serial.print(sread);
     if (ismySerialData) {
        TempInput += sread;
     } else if (sread == '>') {
        TempInput = ">";
        ismySerialData = true;  
     }
     if (sread == '<') {
        ismySerialData = false;
        return TempInput.startsWith(">temp=");
     }
  }
  return false;
}

// ============= Firebase =============================

long Firebase_getResetTime() {
  long resettime = Firebase.getString("garagedoor/resetTime").toInt();

  if (Firebase.failed()) {
      Firebase_fail();
      return 0;
  }
  return resettime;
}

// ------------------------------------------------

bool Firebase_Send_Temperature(float new_temp) {
  // static float last_resettime;
  long resettime = Firebase_getResetTime();
  unsigned long server_epoch = get_Server_Time();

  if ( resettime <= 0 || server_epoch <= 0 ) return false;
     
  unsigned long new_time = server_epoch - resettime;

  if ( new_time > 10000000 ) return false; // if something wrong
      
  String sendstr = "temp:" + String(new_temp) + "&time:" + String(new_time);
  Firebase.pushString("fireplace/data/", sendstr);
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
  //String debug_mode = Firebase.getString("fireplace/debugMode");

  //if (Firebase.failed()) {
  //    Firebase_fail();
  //    return;
  //}
  //if (debug_mode.equals("true")) {
    Firebase.setString("fireplace/debug/", debug_msg);
  //}
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

