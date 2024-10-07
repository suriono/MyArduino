#include <ESP8266WiFi.h>
#include <Wire.h> //Needed for I2C to GNSS
#include <SoftwareSerial.h>

SoftwareSerial mySerial(12, 14); // RX: D6: 12, TX: D5: 14
WiFiClient wificlient;
WiFiServer server(8000);     // port 8000 for incoming wifi

#define DEBUGGING

#include <SparkFun_u-blox_GNSS_Arduino_Library.h> //http://librarymanager/All#SparkFun_u-blox_GNSS
SFE_UBLOX_GNSS myGNSS;

long lastTime = 0; //Simple local timer. Limits amount if I2C traffic to u-blox module.
unsigned long last_incoming_wifi = millis();
byte incomingByte = 0;

long latitude, longitude;
byte SIV;

void setup()
{
  Serial.begin(57600);
  mySerial.begin(19200);
  while (!Serial); //Wait for user to open terminal
  
  WiFi.softAP("FriendlyRoboticsGPS", "helloworld");
  Serial.print("AP IP: "); Serial.println(WiFi.softAPIP());
 // WiFi.enableAP(false);
 // WiFi.mode(WIFI_STA);
 // wifi_begin();
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  wificlient.setTimeout(200);
  
  Wire.begin();

  //myGNSS.enableDebugging(); // Uncomment this line to enable helpful debug messages on Serial

  if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }

  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  //myGNSS.setNavigationFrequency(1); //Set output to 20 times a second
  byte rate = myGNSS.getNavigationFrequency(); //Get the update rate of this module
  Serial.print("Current update rate: ");
  Serial.println(rate);
  
 // myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR
  server.begin();
}

// ========================= LOOP ========================
int incomingRCTM3 = 0;

void loop()
{
  if (incoming_wifi()) {
  //Query module only every second. Doing it more often will just cause I2C traffic.
  //The module only responds when a new position is available
  } else if (millis() - lastTime > 3000)
  {
    lastTime = millis(); //Update the timer
    GPS_high_precision();
  //  latitude = myGNSS.getLatitude();
  //  longitude = myGNSS.getLongitude();
   // SIV = myGNSS.getSIV();

    #ifdef DEBUGGING
   //   Serial.print(F("Latitute, Longitute: "));
  //    Serial.print(latitude); Serial.print(" , "); Serial.print(longitude); // Serial.print(F(" (degrees * 10^-7)"));
      //long altitude = myGNSS.getAltitude();
  //    Serial.print(F(" SIV: "));
  //    Serial.print(SIV);
  //    Serial.println();
    #endif
    
//    if ( wificlient.connect(HOST_IP, HOST_PORT)) {
//      #ifdef DEBUGGING
//        Serial.print(" ......... Sending to Base Station, elapsed: "); Serial.println(millis()- last_incoming_wifi);
//      #endif
//      wificlient.print("OK<");
//      wificlient.flush();
//      last_incoming_wifi = millis();
//    }
    
  } else if (mySerial.available()) {
    //Serial.println("Incoming serial..");
   // while (mySerial.available()) {
      incomingRCTM3 = mySerial.read();
      Serial.print(incomingRCTM3, DEC);
  }
  
}
