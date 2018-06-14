/* NodeMCU pins mapping to Arduino
D0   = 16;    D1   = 5;     D2   = 4;
D3   = 0;     D4   = 2/TX1;     D5   = 14;    D6   = 12;
D7   = 13;    D8   = 15;    D9   = 3;     D10  = 1;
*/

#include <TinyGPS.h>
//#include <math.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
//#include<IPAddress.h>

//#define DEBUG_GPS

#define BUZZER_PIN      14   // D5


TinyGPS gps1;
SoftwareSerial mySerial(4, 5); // RX, TX
#define R_earth 6373000.0    // radius of earth

WiFiServer server(80);
WiFiClient client;

float last_lat, last_lon;
unsigned long last_time;
unsigned long total_distance;
unsigned long last_buzz;        // last time buzz

// routines declarations .......
void get_gps(boolean newData, TinyGPS gps, float *fixlat, float *fixlon);
//double get_distance(float flat1, float flon1, float flat2, float flon2);
void write_client(); // String outstr);
unsigned long Buzz(int,int);

// ************* Setup *************************             
void setup() { 
  
  Serial.begin(57600);
  mySerial.begin(9600);
  
  while(mySerial.available()) mySerial.read();  // read leftover data

  //ESP.reset(); delay(5000);   // if necessary
  ESP.eraseConfig(); delay(1000); // needed if it keeps old setup
  Serial.print("Auto Connect: "); Serial.println(WiFi.getAutoConnect());
 
  WiFi.setOutputPower(1.0);  // 0.5 to 20 Output WiFi power, it's near AP
  WiFi.enableAP(false);
  WiFi.mode(WIFI_STA);
  IPAddress ip(192, 168, 4, 100); 
  IPAddress gateway(192, 168, 4, 1); // set gateway to match your network
  IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your
  WiFi.config(ip, gateway, subnet);
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

  pinMode(BUZZER_PIN, OUTPUT);
  last_buzz = Buzz(100, 300); // freq, amplitude (0-1023)
  delay(1000);
  Buzz(200, 0);  // stop buzz
     
}


// ************** Loop *************************

void loop() {

  bool newData1 = false;
  float lat1, lon1;
  static int numConnectedClient = 0;
  static bool isBuzz;
  static bool isDataSent = false;
  
  while (mySerial.available()) {      //char c=mySerial.read();
     if (gps1.encode(mySerial.read())) {// Did a new valid sentence come in?
        newData1 = true;
     }
  }

  if(newData1) {
     get_gps(newData1, gps1, &lat1, &lon1);
  //} else if (isDataSent) {
   //  isDataSent = false;
     
  } else if ( ((millis() - last_buzz) > 100) && isBuzz ) { // stop buzz
     Buzz(100, 0);  // stop buzzing
     isBuzz = false; 
  }

  // if (isBuzz) Serial.println("Buzzing");

  client = server.available();
  if (client) {

     //write_client();
     //isDataSent = true;
     
     char readchar;
     String readstr, datastr;
     int count = 0;
     bool isStartData = false;
     while(count < 100) {
        while(client.available()) {
           count++;
           readchar = client.read();
           //readstr += readchar;
           if (isStartData) {
              datastr += readchar;
           } else {
              readstr += readchar;
              if (readstr.indexOf("Accept-Encoding: gzip") > 100) {
                 isStartData = true;
                 //datastr += readchar;
              }
           }
        }
        if (count < 100) delay(1);
        count++;
     }
        
     
     //} else {
     write_client();
        
     if (datastr.indexOf("buzz") > 1) {  // sound buzz
        last_buzz = Buzz(100, 1); // freq, amplitude (0-1023)
        isBuzz = true;
        client.print(",RP BUZZ");
     } else if (datastr.indexOf("[[") > 1) {   // receive the fence coordinates
        client.print(",RP OK_FENCE");
     }
        client.flush();
     //}
     
     //write_client();
     //client.flush();
     
     //if (readstr.indexOf("senddata")> 0) {
     //   write_client(); //"Lat=");   
     //}
     datastr.trim();
     Serial.println(readstr);
     Serial.println("===============================================\n");
     Serial.println("client read: " + String(count) + " characters " + datastr);
     //client.flush();
     client.stop();

    
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
    #ifdef DEBUG_GPS
    Serial.print("SAT=");
    //if (gps.satellites() < 10) Serial.print(" ");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    if(gps.hdop() < 100) Serial.print(" ");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    #endif
    if (flat != TinyGPS::GPS_INVALID_F_ANGLE && flon != TinyGPS::GPS_INVALID_F_ANGLE) {
       #ifdef DEBUG_GPS
       Serial.print(" LAT=");
       Serial.print(flat,7);
       Serial.print(" LON=");
       Serial.print(flon,7);
       #endif
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
  #ifdef DEBUG_GPS
  Serial.print(" CSUM ERR=");
  Serial.print(failed);
  Serial.println("");
  #endif
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

// ====================== Buzz ======================
unsigned long Buzz(int freq, int amplitude) {
  analogWriteFreq(freq);
  analogWrite(BUZZER_PIN, amplitude); // 0 to 1023 for PWM duty cycle
  return millis();
}
