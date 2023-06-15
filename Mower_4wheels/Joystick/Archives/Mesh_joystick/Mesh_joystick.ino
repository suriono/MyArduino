#include <Arduino_JSON.h>
#include <SoftwareSerial.h>
#include <painlessMesh.h>

#define   DEBUGING

#define   LED             LED_BUILTIN
#define   MESH_SSID       "UzMesh"
#define   MESH_PASSWORD   "UzRobotic21353"
#define   MESH_PORT        5555

SoftwareSerial ProminiSerial(D2 , D1);     // RX=promini, TX
bool           isIncomingGood = false;     // if the incoming serial JSON good

// Prototypes
void sendMessage(); 
void receivedCallback(uint32_t from, String & msg);
void newConnectionCallback(uint32_t nodeId);
void changedConnectionCallback(); 
void nodeTimeAdjustedCallback(int32_t offset); 
void delayReceivedCallback(uint32_t from, int32_t delay);
JSONVar       myJSON, receivedJSON, generalJSON;
Scheduler     userScheduler; // to control your personal task
painlessMesh  mesh;

bool calc_delay = false;
SimpleList<uint32_t> nodes;

void sendMessage() ;      // Prototype
Task taskSendMessage( TASK_SECOND * 1, TASK_FOREVER, &sendMessage ); // start with a one second interval
bool onFlag = false;      // LED on/off

//String InStr; //  OutStr;

// ============================= Set Up ==========================
void setup() {
  Serial.begin(57600);
  ProminiSerial.begin(57600);
  ProminiSerial.setTimeout(100);
  while (Serial.available()) Serial.read();
  
  generalJSON["lastLED"]  = millis();
  Mesh_Init();
  pinMode(LED, OUTPUT);
}

// ============================= Loop =============================
void loop() {
  static unsigned long last_blink;
  
  mesh.update();
   if ( (millis()-last_blink) > 500) {
      last_blink = millis();
      digitalWrite(LED_BUILTIN, onFlag);
    }
  //digitalWrite(LED, !onFlag);

  Promini_Incoming();
}

// ======================= Sending Message periodically =========================
void sendMessage() {

  /*
    String msg = " Joystick";
    //msg += mesh.getNodeId();
    // msg += " myFreeMemory: " + String(ESP.getFreeHeap());
    mesh.sendBroadcast(msg);

    if (calc_delay) {
      SimpleList<uint32_t>::iterator node = nodes.begin();
      while (node != nodes.end()) {
        mesh.startDelayMeas(*node);
        node++;
      }
      calc_delay = false;
    } 
    */
}

// ======================== New Connection ============================
void newConnectionCallback(uint32_t nodeId) {
  // Reset blink task
  onFlag = false;
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
  Serial.printf("--> startHere: New Connection, %s\n", mesh.subConnectionJson(true).c_str());
}

// ======================= Change Connection ==========================
void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
  // Reset blink task
  onFlag = false;
  
  nodes = mesh.getNodeList();

  Serial.printf("Num nodes: %d\n", nodes.size());
  Serial.printf("Connection list:");

  SimpleList<uint32_t>::iterator node = nodes.begin();
  while (node != nodes.end()) {
    Serial.printf(" %u", *node);
    node++;
  }
  Serial.println();
  calc_delay = true;
}

// ======================= Adjust Time Callback =======================
void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

// ======================= Delay Receive Callback =====================
void delayReceivedCallback(uint32_t from, int32_t delay) {
  Serial.printf("Delay to node %u is %d us\n", from, delay);
}
