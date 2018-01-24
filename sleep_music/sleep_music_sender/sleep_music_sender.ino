/* NodeMCU pins mapping to Arduino
D0   = 16;    D1   = 5;     D2   = 4;
D3   = 0;     D4   = 2;     D5   = 14;    D6   = 12;
D7   = 13;    D8   = 15;    D9   = 3;     D10  = 1;
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>       

#define WIFI_SSID "sleep_AP"
#define WIFI_PASSWORD "sleep1234"
#define localUDPPort  2391      // local port to listen for UDP packets
//#define REMOTE_IP "192.168.254.79"
#define REMOTE_IP "192.168.4.1"
#define REMOTE_PORT 2390

WiFiUDP Udp;

void setup() {
  Serial.begin(115200);

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

 // udp.listen( true );           // and wait for incoming message

}

//const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
const int NTP_PACKET_SIZE = 24; // NTP time stamp is in the first 48 bytes of the message
char packetBuffer[ NTP_PACKET_SIZE]; // = "hello"; //buffer to hold incoming and outgoing packets
//char  ReplyBuffer[] = "acknowledged";       // a string to send back

// ===========================================

int signal_Processing() {

   static int sig[5], sig_index;

   sig_index = (sig_index + 1)%5;

   byte i0 = (sig_index + 1)%5;
   byte i1 = (sig_index + 2)%5;
   byte i2 = (sig_index + 3)%5;
   byte i3 = (sig_index + 4)%5;

   double deriv1 = -sig[sig_index] +  8.0*sig[i3] - 8.0*sig[i1] + sig[i0];
   //double deriv2 = -sig[sig_index] + 16.0*sig[i3] -30.0*sig[i2] + 16.0*sig[i1] - sig[i0];
   
   
   Serial.print(deriv1); Serial.print(",");
   //Serial.print(deriv2); Serial.print(",");
   sig[sig_index] = analogRead(A0);

   return sig[sig_index];
}


void loop() {
  static unsigned long last_time;

  if ( (millis() - last_time) > 10) {  // send packet
    last_time = millis();

    String tmpstr =String (signal_Processing()); 
    tmpstr.toCharArray(packetBuffer, NTP_PACKET_SIZE);

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println(packetBuffer);

      Udp.beginPacket(REMOTE_IP, REMOTE_PORT);
      Udp.write(packetBuffer, NTP_PACKET_SIZE);
      Udp.endPacket();
    } else {
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      Serial.print("connecting");
      while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
      }
    }
  
  }

  delay(10);
  /*
  int packetSize = Udp.parsePacket();
  if (packetSize) {                     // incoming packet data
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i = 0; i < 4; i++) {
      Serial.print(remote[i], DEC);
      if (i < 3) {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.println(packetBuffer);
  
  } 
  */
  
  
}


/*
void receive( byte[] data ) {          // <-- default handler
//void receive( byte[] data, String ip, int port ) {   // <-- extended handler

  for(int i=0; i < data.length; i++)  print(char(data[i]));
    
  println();
}
*/
