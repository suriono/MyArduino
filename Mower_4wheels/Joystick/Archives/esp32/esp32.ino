#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>
#include <SoftwareSerial.h>

#define HOST_IP     "192.168.0.212"
#define HOST_PORT   8000

JSONVar myJSON;
WiFiClient wificlient;
SoftwareSerial ProminiSerial(D2 , D1);     // RX=promini, TX
String InStr; //  OutStr;

#define DEBUG

void setup() {
  #ifdef DEBUG
    Serial.begin(57600);
  #endif
  ProminiSerial.begin(57600);
  ProminiSerial.setTimeout(100);
  WiFi.enableAP(false);
  WiFi.mode(WIFI_STA);
  wifi_begin();
  wificlient.setTimeout(200);
}

void loop() {
  char ch;
  
  //if (ProminiSerial.available()) {
   //InStr  = "{\"mag\":100"   ;   
  //  InStr += ",\"theta\":20";
   // Outstr += ",\"speedmax\":" + String(SP_val);
    //InStr += ",\"delay\":200}";
     InStr = ProminiSerial.readStringUntil('<');
     #ifdef DEBUG
          Serial.print("Incoming: ");Serial.print(InStr);
     #endif
     myJSON = JSON.parse(InStr);
     if ( JSON.typeof(myJSON) == "undefined") { // not a valid JSON
        #ifdef DEBUG
          Serial.println("=====bad JSON");
        #endif
        return;
     }
     //myJSON["speedmax"] = 111;
     //OutStr = JSON.stringify(myJSON);
     
     //#ifdef DEBUG
     //   Serial.print(OutStr);
     //#endif
     
     if (!wificlient.connect(HOST_IP, HOST_PORT)) {
        #ifdef DEBUG
          Serial.println("\t,  Connection to robot failed !!!!");
        #endif
        delay(1);
        return;
     }
     
     // Sending data ======
     
     wificlient.print(InStr + "<");
     #ifdef DEBUG
       Serial.println("\t,   Sending ..");
     #endif
     while (wificlient.available()) {
       ch = wificlient.read();
       #ifdef DEBUG
         Serial.print("\t,  Received:"); Serial.println(ch);
       #endif
     }
     wificlient.flush();

    // ProminiSerial.write("OK<");
  //}
  delay(100);
}
