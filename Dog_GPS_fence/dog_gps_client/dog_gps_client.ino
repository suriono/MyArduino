#include <TinyGPS.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

//#define DEBUG_GPS
#define BUZZ_MIN_DISTANCE_METER 3.0

#define BUZZER_PIN      14   // D5
SoftwareSerial mySerial(4, 5); // RX, TX

TinyGPS gps1;
WiFiServer server(80);
WiFiClient client;

float last_lat, last_lon;
unsigned long last_time_gps;
unsigned long total_distance;
unsigned long last_buzz;        // last time buzz
bool isFenceOK = false;
bool isAutoBuzzer = false;    // if the buzzer is enabled
float Fx[4], Fy[4];             // Fence coordinates
float FvxN[4], FvyN[4];         // Fence normalized wall vectors
double Dist_to_Fence[4];  // distance fences, negative = outside the fence
boolean isGPSfixed = false;


// ************* Setup *************************             
void setup() { 
  Serial.begin(57600);
  mySerial.begin(9600);
  while(mySerial.available()) mySerial.read();  // read leftover data
  pinMode(BUZZER_PIN, OUTPUT);

  WiFi_Setup();                    // WiFi setup followed by buzzer indicator 
  
  isFenceOK = get_Fence();
  if (isFenceOK) {
    Buzz_Delay(100, 300, 500); delay(500); Buzz_Delay(100, 300, 500); 
  }
}

// ************** Loop *************************

void loop() {

  bool newGPSdata = false;
  float lat1, lon1;
  static int numConnectedClient = 0;
  static bool isBuzz;
  static bool isDataSent = false;
  static unsigned long last_newGPSdata;

  while (mySerial.available()) {      //char c=mySerial.read();
     if (gps1.encode(mySerial.read())) {// Did a new valid sentence come in?
        newGPSdata = true;
     }
  }

  if(newGPSdata && (millis() - last_newGPSdata) > 600) { // if any GPS data, GPS only refresh 1 sec
     last_newGPSdata = millis();
     get_gps(newGPSdata, gps1, &lat1, &lon1);
     if( isGPSfixed && isFenceOK) {
        Distance_to_Fence();
        int min_dist = 10000;
        //Serial.println("GPS is fixed and fence is available");
        for (byte nn=0; nn<4 ; nn++) {    // from four walls
           if ( Dist_to_Fence[nn] < min_dist) min_dist = Dist_to_Fence[nn];
        //  Serial.print(","); Serial.print(Dist_to_Fence[nn]);
        }
        Serial.print("The closest wall distance: "); Serial.println( min_dist);
        //Serial.println();
     }
  } else if ( ((millis() - last_buzz) > 100) && isBuzz ) { // stop buzz if buzzing
     Buzz(100, 0);  // stop buzzing
     isBuzz = false; 
  }

  client = server.available();
  if (client) {           // connected to phone client

     char readchar;
     String readstr, datastr;
     int count = 0;
     bool isStartData = false;
     while(count < 100) {    // to account for delay in data streaming in
        while(client.available()) {
           count++;
           readchar = client.read();
           if (isStartData) {
              datastr += readchar;
           } else {
              readstr += readchar;
              if (readstr.indexOf("Accept-Encoding: gzip") > 100) {
                 isStartData = true;
              }
           }
        }
        if (count < 100) delay(1);
        count++;
     }
        
     write_client(); // send lattitude, longitude, and time

     if (isFenceOK) {
        //Distance_to_Fence();  // invoked in GPS_coordinate
        client.print(",RP DISTFENCE");
        for (byte nn=0; nn<4 ; nn++) {    // from four walls
           client.print(" ");
           client.print(Dist_to_Fence[nn]);
        }
     }

     datastr.trim();
     if (datastr.indexOf("buzz") > -1) {  // sound buzz
        last_buzz = Buzz_Delay(100, 1, 1000); // freq, amplitude (0-1023)
        isBuzz = true;
        client.print(",RP BUZZ");
     } else if (datastr.indexOf("autonobuz") > -1) { 
        isAutoBuzzer = false;
     } else if (datastr.indexOf("autobuz") > -1) { 
        isAutoBuzzer = true;
     } else if (datastr.indexOf("get_fence_points") > -1) {   // receive the fence coordinates
        send_Fence();
     } else if (datastr.indexOf("[[") > -1) {   // receive the fence coordinates and write
        write_Fence(datastr);
     }
     client.flush();
     
     Serial.println("===============================================\n");
     Serial.println("client read: " + String(count) + " characters:" + datastr + "======");
     //client.flush();
     client.stop();

  }
}  // ************* end of Loop ***********************
