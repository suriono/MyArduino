#include <ESP8266WiFi.h>

boolean WifiAP_begin();

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
  boolean isAP_ready = WifiAP_begin();
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
