/* NodeMCU pins mapping to Arduino
D0   = 16;    D1   = 5;     D2   = 4;
D3   = 0;     D4   = 2/TX1;     D5   = 14;    D6   = 12;
D7   = 13;    D8   = 15;    D9   = 3;     D10  = 1;
*/

#include <TinyGPS.h>
#include <math.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

//#define HOME_ROUTER             // during development using home router

TinyGPS gps1;
SoftwareSerial mySerial(4, 5); // RX, TX
#define R_earth 6373000.0    // radius of earth

WiFiServer server(80);
WiFiClient client;

float last_lat, last_lon;
unsigned long last_time;
unsigned long total_distance;

// routines declarations .......
void get_gps(boolean newData, TinyGPS gps, float *fixlat, float *fixlon);
//double get_distance(float flat1, float flon1, float flat2, float flon2);
void write_client(); // String outstr);

// ************* Setup *************************             
void setup() { 
  
  Serial.begin(57600);
  mySerial.begin(9600);
  
  while(mySerial.available())  // until receive GPS serial
    if (mySerial.read() == '\r') 
       break;

  //ESP.reset(); delay(5000);   // if necessary
  ESP.eraseConfig(); delay(1000); // needed if it keeps old setup
  Serial.print("Auto Connect: "); Serial.println(WiFi.getAutoConnect());
 
  WiFi.setOutputPower(1.0);  // 0.5 to 20 Output WiFi power, it's near AP
  WiFi.enableAP(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin("Dog_AP","hellopuppy");
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
    
  // Start the server
  server.begin();
  Serial.println("Server started");
}


// ************** Loop *************************

void loop() {

  bool newData1 = false;
  float lat1, lon1;
  static int numConnectedClient = 0;
  
  while (mySerial.available()) {      //char c=mySerial.read();
     if (gps1.encode(mySerial.read())) {// Did a new valid sentence come in?
        newData1 = true;
     }
  }
  
  if(newData1) {
     get_gps(newData1, gps1, &lat1, &lon1);
  }

  client = server.available();
  if (client) {
     //if (client.connected()) {

     //unsigned long tmp_time = millis(); // for timeout below
    // while(!client.available() && (millis()-tmp_time) < 1000){
     //  delay(1);
    // }
     
     char readchar;
     String readstr, datastr;
     while(client.available()) {
              //if (inchar > 31) {
              //  instring += inchar;
              //}
        readchar = client.read();
        readstr += readchar;
   
        //if (readchar == '\n') {
        //  readstr = "";
        //}
              //serial_println(readstr);  // print other inputs
           //   readstr = "";
           //} else if (readstr.compareTo("tempo=") == 0) {
              //serial_println("found tempo");
              //serial_println(readstr);
           //   readstr = "";
           //}
     }
     write_client(); //"Lat=");
     //write_client_float(last_lat);
     //Serial.println("Lat=" + convertFloatToString(last_lat) + "====");
     //Serial.println(last_lat, 8);
     Serial.println("client read: " + readstr);
     //client.flush();
     //client.stop();
  }
}  // ************* end of Loop ***********************


// ***************** get gps **********************************
void get_gps(boolean newData, TinyGPS gps, float *fixlat, float *fixlon) {
  unsigned long chars;
  unsigned short sentences, failed;
  float flat, flon;      // fix lattitue and longitude
  unsigned long fix_age; // returns +- latitude/longitude in degrees
  gps.f_get_position(&flat, &flon, &fix_age);
  *fixlat = flat;
  *fixlon = flon;
  if (fix_age == TinyGPS::GPS_INVALID_AGE)
     Serial.println("No fix detected");
  else if (fix_age > 5000)
     Serial.println("Warning: possible stale data!");
     
  if (newData) {
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    //Serial.print("LAT=");
    //Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    //Serial.print(" LON=");
    //Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print("SAT=");
    //if (gps.satellites() < 10) Serial.print(" ");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    if(gps.hdop() < 100) Serial.print(" ");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    if (flat != TinyGPS::GPS_INVALID_F_ANGLE && flon != TinyGPS::GPS_INVALID_F_ANGLE) {
       Serial.print(" LAT=");
       Serial.print(flat,7);
       Serial.print(" LON=");
       Serial.print(flon,7);
       last_time = millis();
       last_lat = flat;
       last_lon = flon;
    }
  }
  
  gps.stats(&chars, &sentences, &failed);
  //Serial.print(" CHARS=");
  //Serial.print(chars);
  //Serial.print(" SENTENCES=");
  //Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.print(failed);
  Serial.println("");
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");
}  // *********** end of get_gps *********************

/*

// ************** Get distance *************************
double get_distance(float flat1, float flon1, float flat2, float flon2) {
   double dlat = (flat1 - flat2)*PI/360.0; // and divided by 2
   double dlon = (flon1 - flon2)*PI/360.0; // and divided by 2
   
   Serial.print(" dlat="); Serial.print(dlat,8);
   Serial.print(" dlon="); Serial.print(dlon,8);
   
   double acord = sin(dlat)*sin(dlat)+cos(flat1*PI/180.0)*cos(flat2*PI/180.0)*sin(dlon)*sin(dlon);
          //double alat = sin(dlat)*sin(dlat);   // lattitue a
          //double alon = cos(flat*PI/180.0)*cos(firstflat*PI/180.0)*sin(dlon)*sin(dlon); // longitude a
          //double clat = 2.0*atan2(sqrt(alat),sqrt(1.0-alat));
          //double clon = 2.0*atan2(sqrt(alon),sqrt(1.0-alon));
   double ccord = 2.0*atan2(sqrt(acord),sqrt(1.0-acord));
   double distance = R_earth * ccord;
          //double distlat = R_earth * clat;
          //double distlon = R_earth * clon;
          
   Serial.print(" distance=");
   Serial.print(distance);
          //Serial.print(" d_lat=");
          //Serial.print(distlat,6);
          //Serial.print(" d_lat_avg=");
          //Serial.print(Avg_Lat(distlat),6);
          //Serial.print(" d_lon=");
          //Serial.print(distlon,6);
          
       //}
   return distance;

} // *********** end of get_distance **********************
*/
// ============= Write to Client =================
void write_client() {
  // send a standard http response header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");  // the connection will be closed after completion of the response
  //client.println("Refresh: 10");  // refresh the page automatically every 5 sec
  client.println();
  //client.println("<!DOCTYPE HTML>");
  //client.println("<html>");

  //unsigned long sendtime = millis();

  client.print("LT ");
  client.print(last_lat,8);
  client.print(",LN ");
  client.print(last_lon,8);
  client.print(",TM ");
  client.print(last_time);

  //client.println("<br/></html>");
}

