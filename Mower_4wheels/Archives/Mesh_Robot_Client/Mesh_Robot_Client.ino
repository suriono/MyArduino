#include <Arduino_JSON.h>
#include <painlessMesh.h>
#include <SabertoothSimplified.h>
#include <SoftwareSerial.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

#define   DEBUGGING

// IMU ====
#define INTERVAL_IMU  500   // interval of reading IMU
MPU6050 mpu;  // AD0 = LOW
float   YAW;                // Yaw value
#define INTERRUPT_PIN  2    // use pin 2 on Arduino Uno & most boards
bool IMU_Ready = false;     // set true if DMP init was successful
uint8_t fifoBuffer[64];     // FIFO storage buffer
Quaternion q;               // [w, x, y, z]         quaternion container
VectorFloat gravity;        // [x, y, z]            gravity vector
float ypr[3];               // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
//bool IMU_pingpong = false;  // alternate to see if data sent


#define   MESH_SSID       "UzMesh"
#define   MESH_PASSWORD   "UzRobotic21353"
#define   MESH_PORT        5555

SoftwareSerial SerialRobot(12 , 13);     // RX, TX
SabertoothSimplified ST(SerialRobot);

// Prototypes
void PeriodicRun(); 
void receivedCallback(uint32_t from, String & msg);
void newConnectionCallback(uint32_t nodeId);
void changedConnectionCallback(); 
void nodeTimeAdjustedCallback(int32_t offset); 
void delayReceivedCallback(uint32_t from, int32_t delay);
JSONVar       myJSON, timeJSON;
Scheduler     userScheduler; // to control your personal task
painlessMesh  mesh;

bool calc_delay = false;
SimpleList<uint32_t> nodes;

void PeriodicRun() ;      // Prototype
Task taskPeriodicRun( TASK_SECOND * 1, TASK_FOREVER, &PeriodicRun ); // start with a one second interval
bool LEDonoff = false;      // LED on/off
unsigned long last_valid_json = millis();

//void motorStop();
//void Mesh_Init();
//void IMU_begin();
//void IMU_Yaw();
// ============================= Set Up ==========================
void setup() {
  SerialRobot.begin(38400);
  Serial.begin(57600);
  motorStop();
  Mesh_Init();
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
  IMU_begin();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

// ============================= Loop =============================
void loop() {
  static unsigned long last_valid_json;
  static unsigned long last_blink;

  mesh.update();
  
  if (IMU_Ready) {
    IMU_Yaw();
    if ( (millis()-last_blink) > 500) {
      last_blink = millis();
      digitalWrite(LED_BUILTIN, LEDonoff);
    }
  }
  
}

// ======================= Sending Message Periodically =========================
void PeriodicRun() {
  if ( (millis() - last_valid_json) > 1000) { // stop when no signal
    motorStop();
  }
  /*
  String msg = "Robot from node ";
  msg += mesh.getNodeId();
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

  //Serial.printf("Sending message: %s\n", msg.c_str());
  
  //taskPeriodicRun.setInterval( random(TASK_SECOND * 1, TASK_SECOND * 5));  // between 1 and 5 seconds
  //taskPeriodicRun.setInterval( TASK_SECOND * 1);  // every 1 second
  */
}

// ======================== New Connection ============================
void newConnectionCallback(uint32_t nodeId) {
  // Reset blink task
  //onFlag = false;
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
  Serial.printf("--> startHere: New Connection, %s\n", mesh.subConnectionJson(true).c_str());
}

// ======================= Change Connection ==========================
void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
  // Reset blink task
  //onFlag = false;
  
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
