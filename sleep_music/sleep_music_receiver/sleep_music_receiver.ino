/* NodeMCU pins mapping to Arduino
D0   = 16;    D1   = 5;     D2   = 4;
D3   = 0;     D4   = 2;     D5   = 14;    D6   = 12;
D7   = 13;    D8   = 15;    D9   = 3;     D10  = 1;
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>       

#define WIFI_SSID "xxxx"
#define WIFI_PASSWORD "xxxx"
#define localUDPPort  2390      // local port to listen for UDP packets

WiFiUDP Udp;
// buffers for receiving and sending data
#define NTP_PACKET_SIZE   24
//char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  //buffer to hold incoming packet,
char packetBuffer[NTP_PACKET_SIZE];  //buffer to hold incoming packet,
//char  ReplyBuffer[] = "acknowledged";       // a string to send back

//#define DEBUG

void setup() {
  Serial.begin(115200);

   // connect to wifi.
  WiFi.enableAP(false);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(".");
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
  //Serial.print("Packet size:");
  //Serial.println(UDP_TX_PACKET_MAX_SIZE);
  //delay(10000);
}



void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {

    IPAddress remote = Udp.remoteIP();
    // read the packet into packetBufffer
    //Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    Udp.read(packetBuffer,NTP_PACKET_SIZE);
    
    #ifdef DEBUG
      Serial.print("Received packet of size ");
      Serial.println(packetSize);
      Serial.print("From ");
      
      for (int i = 0; i < 4; i++) {
        Serial.print(remote[i], DEC);
        if (i < 3) {
          Serial.print(".");
        }
      }
      Serial.print(", port ");
      Serial.println(Udp.remotePort());
      Serial.println("Contents:");
    #endif
    Serial.println(packetBuffer);

    // send a reply to the IP address and port that sent us the packet we received
    //Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    //Udp.write(ReplyBuffer);
    //Udp.endPacket();
  }
  delay(1);

}
