#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>
#include <Wire.h> // Needed for I2C to GNSS
#include <SparkFun_u-blox_GNSS_Arduino_Library.h> //http://librarymanager/All#SparkFun_u-blox_GNSS

#define myWire Wire // This will work on the Redboard Artemis and the Artemis Thing Plus using Qwiic

JSONVar myJSON;
WiFiClient wificlient;
WiFiServer server(8000);

SFE_UBLOX_GNSS myGNSS;

long lastTime = 0; //Simple local timer. Limits amount if I2C traffic to u-blox module.
int32_t latitude, longitude, accuracy;
int8_t latitudeHp, longitudeHp;
double d_lat, d_lon;
float f_accuracy;

void setup()
{
  Serial.begin(57600);
  while (!Serial); //Wait for user to open terminal
  myWire.begin();

  WiFi.enableAP(false);
  WiFi.mode(WIFI_STA);
  wifi_begin();
  server.begin();
  
  //myGNSS.enableDebugging(Serial); // Uncomment this line to enable debug messages

  if (myGNSS.begin(myWire) == false) //Connect to the u-blox module using Wire port
  {
    Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1)
      ;
  }

  // Check that this platform supports 64-bit (8 byte) double
  if (sizeof(double) < 8)
  {
    Serial.println(F("Warning! Your platform does not support 64-bit double."));
    Serial.println(F("The latitude and longitude will be inaccurate."));
  }

  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)

  //myGNSS.setNavigationFrequency(20); //Set output to 20 times a second

  byte rate = myGNSS.getNavigationFrequency(); //Get the update rate of this module
  Serial.print("Current update rate: ");
  Serial.println(rate);

  //myGNSS.saveConfiguration(); //Save the current settings to flash and BBR
}

// =========================== LOOP ================================

void loop() {
 // static long last_gps = millis();
  //Query module only every second.
  //The module only responds when a new position is available.
  if (incoming_wifi()){
        json_Process() ;
    
//  } else if (millis() - lastTime > 10000) {
//    lastTime = millis(); //Update the timer

   // Print the lat and lon
   // Serial.print("  Lat (deg): "); Serial.print(d_lat, 9);
   // Serial.print(", Lon (deg): "); Serial.print(d_lon, 9);
   // Serial.print(", Accuracy (mm): "); Serial.println(accuracy, 0); // Print the accuracy with 4 decimal places
  } else {
    //Serial.println( (millis() - last_gps));
   // last_gps = millis();
    get_Coordinates(); 
  }

  
}
