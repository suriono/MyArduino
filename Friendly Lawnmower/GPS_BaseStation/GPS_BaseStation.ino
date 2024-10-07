#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

#define DEBUGGING
//#include <Wire.h>

SoftwareSerial mySerial(14, 12); // RX: D5: 14, TX: D6: 12
WiFiClient wificlient;
#define HOST_IP        "192.168.0.27"
#define HOST_PORT      8000
//WiFiServer server(8000);     // port 8000 for incoming wifi
unsigned long last_basestation = millis();
int incomingByte = 0;
//#include <SparkFun_u-blox_GNSS_Arduino_Library.h> //http://librarymanager/All#SparkFun_u-blox_GNSS
//SFE_UBLOX_GNSS myGNSS;

void setup() {
 // Wire.begin();
 // Wire.setClock(400000); //Increase I2C clock speed to 400kHz
  Serial.begin(56700);
  mySerial.begin(57600);
  WiFi.enableAP(false);
  WiFi.mode(WIFI_STA);
  wifi_begin();
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  wificlient.setTimeout(1000);

  while (Serial.available()) Serial.read(); //Clear any latent chars in serial buffer
  
  while (mySerial.available()) mySerial.read(); //Clear any latent chars in serial buffer
  // put your setup code here, to run once:

 // if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
 // {
 //   Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
 //   while (1);
 // }

//    bool response = true;
  //response &= myGNSS.enableRTCMmessage(UBX_RTCM_1005, COM_PORT_I2C, 1); 

 // if (response == true)
 // {
//    Serial.println(F("RTCM messages enabled"));
//  }
//  else
//  {
 //   Serial.println(F("RTCM failed to enable. Are you sure you have an ZED-F9P?"));
 //   while (1); //Freeze
//  }
  //server.begin();
}

char cinput;
bool serial_available;

void loop() {
  static byte counter = 0;
 // Serial.print("Sending: ");
 // serial_available = mySerial.available();
  if (mySerial.available()) {
   // while (mySerial.available()) {
      incomingByte = mySerial.read();
      
      if ( wificlient.connect(HOST_IP, HOST_PORT)) {
        wificlient.write(incomingByte);
        wificlient.flush();
        #ifdef DEBUGGING
           Serial.print(incomingByte,DEC); Serial.print(',');
           counter = (counter + 1) % 40; 
           if (counter < 1) {
              Serial.println();
           }
        #endif
      }
      
    //}
//    cinput = (char) mySerial.read();
 //   Serial.write(cinput);
   // Serial.println(mySerial.readString());
    
     last_basestation = millis();
  }

//  if (( millis() - last_basestation) > 500) {
//     last_basestation = millis();
//    #ifdef DEBUGGING
//       
//      Serial.println(" ==== Finished reading the Base Station RCTM3 ===="); 
//    #endif
//  }
 // incoming_wifi();
}
