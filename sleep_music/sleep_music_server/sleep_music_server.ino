/* NodeMCU pins mapping to Arduino
D0   = 16;    D1   = 5;     D2   = 4;
D3   = 0;     D4   = 2;     D5   = 14;    D6   = 12;
D7   = 13;    D8   = 15;    D9   = 3;     D10  = 1;
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>       

WiFiServer server(8000);
WiFiClient client;

//#define WIFI_SSID "xxxx"
//#define WIFI_PASSWORD "xxxx"
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

  // Starting WiFi AP server
  Serial.print("Setting soft-AP ... ");
  boolean isAP_ready = WiFi.softAP("sleep_AP", "sleep1234");
  Serial.println(WiFi.softAPIP());
  if(isAP_ready) {
    Serial.println("Access Point Ready");
  } else {
    Serial.println("Access PointFailed!");
  }
  server.begin();
  Serial.println("Server started");

  // Start UDP
  Serial.println("Starting UDP");
  Udp.begin(localUDPPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  
}


unsigned long last_time, last_time1;
void loop() {
  static String OutStr;
  static unsigned long last_LED_on;
  
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

    // read the packet into packetBufffer
    // Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    //Serial.print(String(packetSize) + ",");
    //Udp.read(packetBuffer,NTP_PACKET_SIZE);
    Udp.read(packetBuffer, packetSize);// UDP_TX_PACKET_MAX_SIZE); // packetSize);
    
    

    String readstr = String(packetBuffer);
    //Serial.println(readstr);
    if (readstr.startsWith("data=")) {
      Serial.println(readstr.substring(5).toInt());
    } else if (readstr.startsWith("HeartBeat")) {
      digitalWrite(LED, LOW);
      Serial.println(readstr);
      last_LED_on = millis();
      delay(10);
      digitalWrite(LED, HIGH);
    } 
    

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
  }

  //delayMicroseconds(100);
  delay(10);
 
}
