/* NodeMCU pins mapping to Arduino
D0   = 16;    D1   = 5;     D2   = 4;
D3   = 0;     D4   = 2;     D5   = 14;    D6   = 12;
D7   = 13;    D8   = 15;    D9   = 3;     D10  = 1;
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>       

#define WIFI_SSID "sleep_AP"
#define WIFI_PASSWORD "sleep1234"
#define localUDPPort  2390      // local port to listen for UDP packets


WiFiUDP Udp;
#define LED D0

// buffers for receiving and sending data
#define NTP_PACKET_SIZE   24
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  //buffer to hold incoming packet,
//char packetBuffer[NTP_PACKET_SIZE];  //buffer to hold incoming packet,
char replyBuffer[NTP_PACKET_SIZE];
//#define DEBUG

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);   // LED pin as output. 
  digitalWrite(LED, HIGH); 
  /*
  // Starting WiFi AP server
  Serial.print("Setting soft-AP ... ");
  boolean isAP_ready = WiFi.softAP("sleep_AP", "sleep1234");
  Serial.println(WiFi.softAPIP());
  if(isAP_ready) {
    Serial.println("Access Point Ready");
  } else {
    Serial.println("Access PointFailed!");
  }
  */
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
 

  // Start UDP
  Serial.println("Starting UDP");
  Udp.begin(localUDPPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  
}

// ========================= LOOP ==========================

unsigned long last_time, last_time1;
void loop() {
  static String OutStr;
  static unsigned long last_LED_on;
  static byte counter;
  
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
    
    //Serial.println(readstr);
    //digitalWrite(LED, LOW);
    //delay(100);
    //digitalWrite(LED, HIGH);

      
    if (readstr.startsWith("data=")) { // raw data, e.g. analog, slope
      Serial.println(readstr.substring(5).toInt());
      
    } else if (readstr.startsWith("HeartBeat")) { // trigger heart pulse
      Serial.print(readstr); Serial.print(" "); Serial.println(counter++);
      last_LED_on = millis();
      LED_Blink();
      //Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      //Udp.write("OK");
      //Udp.endPacket();
      
    } else if (readstr.startsWith("interval=")) { // information
      Serial.println(readstr);
      LED_Blink();
      //Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      //Udp.write("OK");
      //Udp.endPacket();
    } 

    //Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    //Udp.write("OK");
    //Udp.endPacket();

    if (OutStr.length() > 0) { // sending data type request
      //char replyBuffer[NTP_PACKET_SIZE];
      OutStr.toCharArray(replyBuffer, NTP_PACKET_SIZE);
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write(replyBuffer);
      Udp.endPacket();
      OutStr = "";   // reset string
    }
    
  } else if (Serial.available()) {  // manual setting incoming type
    while (Serial.available()) {
      OutStr += char(Serial.read());
    }
    /*
  } else if (WiFi.status() != WL_CONNECTED) {
     //while (WiFi.status() != WL_CONNECTED) {
     Serial.println("Reconnecting ...");
     WiFi.connect();
     delay(5000);
     //}
     Serial.print("connected: ");
     Serial.println(WiFi.localIP());
     delay(500);
     */
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

