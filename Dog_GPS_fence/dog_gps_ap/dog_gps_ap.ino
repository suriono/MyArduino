/* NodeMCU pins mapping to Arduino
D0   = 16;    D1   = 5;     D2   = 4;
D3   = 0;     D4   = 2/TX1;     D5   = 14;    D6   = 12;
D7   = 13;    D8   = 15;    D9   = 3;     D10  = 1;
*/

#include <ESP8266WiFi.h>


// ************* Setup *************************             
void setup() { 
  
  Serial.begin(57600);


  //ESP.reset(); delay(5000);
  ESP.eraseConfig(); delay(1000);
  Serial.print("Auto Connect: "); Serial.println(WiFi.getAutoConnect());
  //WiFi.disconnect();
       
  //WiFi.setAutoConnect(false);
  WiFi.enableAP(true);
  //WiFi.scanNetworks(false);
  //WiFi.persistent(true);
  WiFi.mode(WIFI_AP);
  boolean isAP_ready = WiFi.softAP("Dog_AP", "hellopuppy"); delay(500);
  Serial.println(WiFi.softAPIP());
  if(isAP_ready) {
    Serial.println("Access Point Ready");
  } else {
    Serial.println("Access PointFailed!");
  }
  Serial.println(WiFi.SSID());

  
}



// ************** Loop *************************

void loop() { 
}  // ************* end of Loop ***********************

