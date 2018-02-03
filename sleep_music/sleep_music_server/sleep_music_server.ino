/* NodeMCU pins mapping to Arduino
D0   = 16;    D1   = 5;     D2   = 4;
D3   = 0;     D4   = 2/TX1;     D5   = 14;    D6   = 12;
D7   = 13;    D8   = 15;    D9   = 3;     D10  = 1;
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>  
#include "fauxmoESP.h"
#include "pitches.h"   

#define WIFI_SSID "philip"
#define WIFI_PASSWORD "xxxx"
//#define WIFI_SSID "sleep_AP"
//#define WIFI_PASSWORD "sleep1234"
#define localUDPPort  2390      // local port to listen for UDP packets


WiFiUDP Udp;
fauxmoESP fauxmo;               // Alexa ESP
#define LED D0

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE/128];  //buffer to hold incoming packet,
char replyBuffer[UDP_TX_PACKET_MAX_SIZE/128];
//#define DEBUG

void setup() {
  Serial.begin(115200);
  Serial1.begin(31250);       // for MIDI pin D4 NodeMCU
  pinMode(LED, OUTPUT);       // LED pin as output. 
  digitalWrite(LED, HIGH); 

  
  // connect to wifi.
  WiFi.setOutputPower(20.0);          // 0.5 to 20 Output WiFi power
  WiFi.enableAP(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  // Start UDP
  Serial.println("Starting UDP");
  Udp.begin(localUDPPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());

  fauxmo.addDevice("Sleeper Machine");
  fauxmo.onMessage(Alexa_callback);
}

// ========================= LOOP ==========================

unsigned long last_time, last_time1;
void loop() {
  static String OutStr;
  static unsigned long last_LED_on;
  static byte counter;


  // Alexa =========
  fauxmo.handle();
  
  
  #ifdef DEBUG
    static long count_station_connected;
    if ( (millis() - last_time) > 3000) { // stop motor with 1 sec not getting a cmd
      Serial.printf("%d. Stations connected = %d, ", count_station_connected, WiFi.softAPgetStationNum());
      Serial.println();
      Serial1.print("No incoming instruction.");
      last_time = millis();
      count_station_connected++;
    }
  #endif

  int packetSize = Udp.parsePacket();
  if (packetSize) {

    Udp.read(packetBuffer, packetSize);// UDP_TX_PACKET_MAX_SIZE); // packetSize);
    
    String readstr = String(packetBuffer);
    
      
    if (readstr.startsWith("data=")) { // raw data, e.g. analog, slope
      Serial.println(readstr.substring(5).toInt());
      
    } else if (readstr.startsWith("HeartBeat")) { // trigger heart pulse
      //Serial.print(readstr); Serial.print(" "); Serial.println(counter++);
      last_LED_on = millis();
      LED_Blink();
      
      Play_Music();
      
    } else if (readstr.startsWith("interval=")) { // information
      Serial.println(readstr);
      LED_Blink();

      Play_Music();
      
    } else if ( (millis()-last_time_Play_Music) > 300 && isPlay_Music ) { // stop playing
      noNote();
    } 


    if (OutStr.length() > 0) { // sending data type request
      
      OutStr.toCharArray(replyBuffer, OutStr.length()+2);
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write(replyBuffer);
      Udp.endPacket();
      OutStr = "";   // reset string
    }
    
  } else if (Serial.available()) {  // manual setting incoming type
    while (Serial.available()) {
      OutStr += char(Serial.read());
    }
  }

  //delayMicroseconds(200);
  delay(1);
 
}

// ================ Blink LED =============
void LED_Blink() {
  digitalWrite(LED, LOW);
  delay(10);
  digitalWrite(LED, HIGH);
}


// -----------------------------------------------------------------------------
// Alexa Device Callback
// -----------------------------------------------------------------------------
void Alexa_callback(uint8_t device_id, const char * device_name, bool state) {
  Serial.print("Device "); Serial.print(device_name); 
  
  //Switching action on detection of device name
  if ( (strcmp(device_name, "Sleeper Machine") == 0) ) {

    Serial.print(" state: ");
    if (state) {
      Serial.println("ON");
    } else {
      Serial.println("OFF");
    }
    Play_Sound_OnOf = state;
    
  }
}
