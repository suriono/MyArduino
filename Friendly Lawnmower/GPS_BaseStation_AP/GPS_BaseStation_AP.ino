#include <ESP8266WiFi.h>
//#include <SoftwareSerial.h>

#define DEBUGGING
//#include <Wire.h>

//SoftwareSerial mySerial(14, 13); // RX: D5: 14, TX: D6: 12
//SoftwareSerial mySerial2(16, 12); // RX: D5: 14, TX: D6: 12
WiFiClient wificlient;
//#define HOST_IP        "192.168.0.27"
#define HOST_IP        "192.168.4.1"
#define HOST_PORT      8000
//WiFiServer server(8000);     // port 8000 for incoming wifi
unsigned long last_basestation = millis();
int incomingByte = 0;
//#include <SparkFun_u-blox_GNSS_Arduino_Library.h> //http://librarymanager/All#SparkFun_u-blox_GNSS
//SFE_UBLOX_GNSS myGNSS;

#define BUFFERSIZE 2
uint8_t buf[BUFFERSIZE];
int num = 0;

void setup() {
 // Wire.begin();
 // Wire.setClock(400000); //Increase I2C clock speed to 400kHz
  Serial.begin(19200);
  Serial.swap();
  //delay(1000);
  //Serial1.begin(9600);
  //mySerial.begin(19200);
  //mySerial2.begin(19200);
  WiFi.enableAP(false);
  WiFi.mode(WIFI_STA);
  wifi_begin();
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  wificlient.setTimeout(1000);

  //Serial.println("Starting Serial ....");

 // while (Serial.available()) Serial.read(); //Clear any latent chars in serial buffer

//  Serial.println("Starting Software Serial");
 // while (mySerial.available()) mySerial.read(); //Clear any latent chars in serial buffer
 // Serial.println("Starting polling ...");
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

// ============================= Loop =============================

char cinput;
bool serial_available;

void loop() {

  if (Serial.available()) {
   //  uint8_t incoming = Serial.read();
    // buf[num] = uint8_t(incoming);
    // num++;
   // Serial.println("hello");
  //  Serial.print(Serial.read(), DEC);
  // if (num == BUFFERSIZE) {
   //   num = 0;
   
      
       if ( wificlient.connect(HOST_IP, HOST_PORT)) {
           if (wificlient.connected()) {
          //  uint8_t incoming = Serial.read();
               // Serial.print(incoming,DEC); 
                //Serial.println();
                //delay(1);
                //delayMicroseconds(10); 
               wificlient.write(uint8_t(Serial.read())); // incoming);
              // wificlient.write(buf,BUFFERSIZE);
              // Serial.readBytes(buf,1);
             //  wificlient.write(buf,1);
            //   wificlient.flush();
       //      wificlient.write(Serial.read());
           }
      // }
    }
  }
}
//  static byte counter = 0;
//  char buff[8];
//  
//  if (mySerial.available()) {
//      incomingByte = mySerial.read();
//      last_basestation = millis();
//      
//      if ( wificlient.connect(HOST_IP, HOST_PORT)) {
//
//         if (wificlient.connected()) {
//               //wificlient.write(incomingByte);
//               //wificlient.flush();
//               mySerial2.write(incomingByte);
//               mySerial2.flush();
//               #ifdef DEBUGGING
//             //     if ( (millis()-last_basestation) > 100) {
//                     //if (counter < 1) {
//              //       Serial.println("-------------");
//             //     }
//                  Serial.print(incomingByte,DEC); Serial.print(',');
//               #endif
//            //}
//             //}  while (mySerial.available()) ;
//         }
//      }
//     last_basestation = millis();
//  } else {
//    delay(1);
//    //Serial.println("No GPS RCTM3");
//  }
//}
