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

WiFiUDP udp;

#define DEBUG_INTERVAL 60000          // Firebase debug update

String TempInput = "";

unsigned long last_server_epoch, last_server_epoch_elapsed;


#define DEBUG

// ========================== Setup =========================

void setup() {
  Serial.begin(57600);
  TempSerial.begin(9600);    // Temperature serial

  // connect to wifi.
  ESP.eraseConfig(); delay(1000);
  WiFi.enableAP(false);
  WiFi.mode(WIFI_STA);
   // connect to wifi.
  WiFi.enableAP(false);
  WiFi_begin();
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(". check the wifi ssid and password");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase_begin();

  Udp_begin();

  Firebase_debug("Restarting Home Temp");
  delay(1000);
}

// ========================== Loop ==========================

void loop() {
  static unsigned long last_debug_time;
  
  static float new_temperature;
  static boolean isTempAvailable = false;

  if (read_Temperature()) {
     Serial.print("Serial input:");
     Serial.println(TempInput);
     Serial.println(TempInput.substring(6,TempInput.indexOf('<')));
     new_temperature = TempInput.substring(6,TempInput.indexOf('<')).toFloat();
     Firebase_Send_Temperature(new_temperature);
  } else if ((millis() - last_debug_time) > DEBUG_INTERVAL) {


    //unsigned long tmp_time = get_Server_Time();
    //if (tmp_time < 199) { // no data packet for server time
  //     tmp_time = last_server_epoch + (millis() - last_server_epoch_elapsed)/1000; // 199 just arbitrary 
//    }
    Firebase_debug("T@" + String(new_temperature) + "^time@" + String(get_server_epoch()));
    last_debug_time = millis();
  }
  delay(100);
}

// ============= Read Temperature =====================

boolean read_Temperature() {
  char sread;
  static boolean ismySerialData = false;
  
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
