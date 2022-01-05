
#include <Arduino.h>
#if defined ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#elif defined ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#error Wrong platform
#endif 

#include <WifiLocation.h>
char* googleApiKey;

float Lattitude, Longitude;
int   Accuracy;
String Response;

WiFiServer server(80);
WiFiClient client;

// ==================== Setup ===========================================

void setup() {
   Serial.begin(57600);
   WiFi_Setup();
}

// ==================== Loop =============================================
void loop() {
   static byte nn;
   client = server.available();
   if (!client) {
      delay(200);
      return;
   }
   Serial.println(nn++);
    
   char readchar;
   String readstr, datastr;
   int count = 0;
   int readcount = 0;
   bool isStartData = false;
   while(client.connected() && count < 1000) {
      //while(count < 100) {    // to account for delay in data streaming in
      //while(client.available()) {
      if(client.available()) {
         count++;
         readchar = client.read();
         //Serial.print(readchar);
         if (isStartData) {
            datastr += readchar;
         } else {
            readstr += readchar;
            if (readstr.indexOf("Accept-Encoding: gzip") > 100) {
               isStartData = true;
            }
         }
      } else {
         delayMicroseconds(10);  // if it goes too fast
      }
      count++;
   }

   if (datastr.indexOf("senddata") > 1) {
      Serial.println("Sending data ...............");
      setWiFiLocation();
      WiFi_SendLocation();
   }
   //Serial.println(datastr.indexOf("senddata"));
   //Serial.println("==========");Serial.print("datastr: "); Serial.print(datastr); Serial.println("========");
   //Serial.print("Count: "); Serial.println(count);
  // }
}
