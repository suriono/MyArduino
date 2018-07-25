/* NodeMCU pins mapping to Arduino
D0   = 16;    D1   = 5;     D2   = 4;
D3   = 0;     D4   = 2/TX1;     D5   = 14;    D6   = 12;
D7   = 13;    D8   = 15;    D9   = 3;     D10  = 1;
*/

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
bool isBuzzerEnable = false;    // if the buzzer is enabled
float Fx[4], Fy[4];             // Fence coordinates
float FvxN[4], FvyN[4];         // Fence normalized wall vectors
double Dist_to_Fence[4];  // distance fences, negative = outside the fence


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
  Buzz_Stop();

  isFenceOK = get_Fence();
     
}


// ************** Loop *************************

void loop() {

  bool newGPSdata = false;
  float lat1, lon1;
  static int numConnectedClient = 0;
  static bool isBuzz;
  static bool isDataSent = false;
  
  while (mySerial.available()) {      //char c=mySerial.read();
     if (gps1.encode(mySerial.read())) {// Did a new valid sentence come in?
        newGPSdata = true;
     }
  }

  if(newGPSdata) {
     get_gps(newGPSdata, gps1, &lat1, &lon1);
  } else if ( ((millis() - last_buzz) > 100) && isBuzz ) { // stop buzz
     Buzz(100, 0);  // stop buzzing
     isBuzz = false; 
  }

  client = server.available();
  if (client) {

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
        
     
     //} else {
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
        last_buzz = Buzz(100, 1); // freq, amplitude (0-1023)
        isBuzz = true;
        client.print(",RP BUZZ");
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


