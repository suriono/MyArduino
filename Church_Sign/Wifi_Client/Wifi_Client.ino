//#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>

SoftwareSerial SerialSoft(0, 5); // RX, TX

WiFiServer server(80);
WiFiClient client;

char* get_SSID();
String get_WiFi_Password();

String Incoming_Data;

void setup() {
  Serial.begin(9600);
  //Serial1.begin(9600);
  SerialSoft.begin(9600);
  while(Serial.available()) Serial.read();  // read leftover data

  //ESP.eraseConfig(); delay(1000); // needed if it keeps old setup
  Serial.print("Auto Connect: "); Serial.println(WiFi.getAutoConnect());

  WiFi.setOutputPower(10.0);  // 0.5 to 20 Output WiFi power, it's near AP
  WiFi.enableAP(false);
  WiFi.mode(WIFI_STA);
  //IPAddress ip(192, 168, 4, 100); 
  //IPAddress gateway(192, 168, 4, 1); // set gateway to match your network
  //IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your
  //WiFi.config(ip, gateway, subnet);
//  WiFi.begin(ssid, PASSWORD);
  //WiFi.begin(SSID_NAME, PASSWORD);
  Wifi_Begin();  // see the password file not in the github
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
    
  // Start the server
  server.begin();
  delay(2000);
  Serial.println("Server started");
}

void loop() {
  client = server.available();
  if (client) {
    incoming_client();
  } else {
    delay(10);
  }
}
