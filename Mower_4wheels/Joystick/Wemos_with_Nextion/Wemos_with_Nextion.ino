#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>
#include "EasyNextionLibrary.h"  // Include EasyNextionLibrary

#define DEBUGGING
#define HOST_IP        "192.168.4.1"
#define HOST_PORT      8000
#define WIFI_INTERVAL  250

JSONVar myJSON;
WiFiClient wificlient;

SoftwareSerial myserial(D6,D7);  // RX,TX In EasyNextionLibrary.h and EasyNextionLibrary.cpp change HardWareSerial -> SoftwareSerial
                                 // also add #include <SoftwareSerial.h> in the library
EasyNex myNex(myserial);       // Create an object of EasyNex class with the name < myNex >
                               // Set as parameter the Hardware Serial you are going to use

int Max_Speed, Mag_raw, Angle_raw, Afwd_ref, Alef_ref,FR_val, LR_val;
                               
// =========================== Setup =============================================                 
void setup() {
  Serial.begin(57600);  
  myNex.begin(9600); // Begin the object with a baud rate of 9600
  WiFi.enableAP(false);
  WiFi.mode(WIFI_STA);
  wifi_begin();
  wificlient.setTimeout(200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(D3, OUTPUT);
  Joystick_get(); delay(500); Joystick_get();
  Afwd_ref = FR_val;
  Alef_ref = LR_val;
  trigger0();
}

// ========================== Loop ==============================================

void loop() {
   myNex.NextionListen(); // WARNING: This function must be called repeatedly to response touch events
   WIFI_outgoing();
   WIFI_incoming();
}
