/* NodeMCU pins mapping to Arduino
D0   = 16;    D1   = 5;     D2   = 4;
D3   = 0;     D4   = 2/TX1;     D5   = 14;    D6   = 12;
D7   = 13;    D8   = 15;    D9   = 3;     D10  = 1;
*/
#include <ESP8266WiFi.h>

//#define DEBUG

//boolean toggle_trimmer = false;   // toggle trimmer on and off

WiFiServer server(8000);
WiFiClient client;

void setup() {
  Serial.begin(57600);
  delay(100);

  pinMode(A0, INPUT);       // to control maximum speed


  Serial1.begin(38400);   // Sabertooth serial, D4->NodeMCU
  Serial1.flush();


  Serial.print("Setting soft-AP ... ");
  boolean isAP_ready = WiFi.softAP("Robot_AP", "skunk4u2");
  Serial.println(WiFi.softAPIP());
  if(isAP_ready) {
    Serial.println("Access Point Ready");
  } else {
    Serial.println("Access PointFailed!");
  }

  // Start the server
  server.begin();
  Serial.println("Server started");

  //client.setTimeout(20);

  
}


// ================ LOOP ========================
unsigned long last_time, last_time1;
void loop() {

  #ifdef DEBUG
  static long count_station_connected;
  if ( (millis() - last_time) > 3000) { // stop motor with 1 sec not getting a cmd
    Serial.printf("%d. Stations connected = %d, ", count_station_connected, WiFi.softAPgetStationNum());
    Serial.println();
    Serial1.print("No incoming instruction.");
    last_time = millis();
    count_station_connected++;
  }
  #endif
  
  // Check if a client has connected
  client = server.available();
  if (!client) {
    delay(1);
    return;
  }

  
  // Wait until the client sends some data
  //Serial.print("Incoming client: ");
  //Serial.print(client.remoteIP());
  //Serial.print(":"); Serial.println(client.remotePort());

  unsigned long tmp_time = millis(); // for timeout below
  while(!client.available()){
    delay(1);
    if ( (millis() - tmp_time) > 1000) return; // timeout after 1 sec
  }

  
  // Read the first line of the request
  //String instring;
  while(client.available()) {
    char inchar = client.read();
    Serial1.write(inchar);
    //if (inchar > 31) {
    //  instring += inchar;
    //}
  }

  last_time = millis();
  //Serial.println("Incoming msg:" + instring);
  
}  // ====== end of loop ==================

