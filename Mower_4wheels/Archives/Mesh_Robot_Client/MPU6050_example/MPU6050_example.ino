#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"


#define INTERVAL_IMU  100   // interval of reading IMU
MPU6050 mpu;  // AD0 = LOW
float   YAW;
#define INTERRUPT_PIN  2  // use pin 2 on Arduino Uno & most boards
// MPU control/status vars
bool IMU_Ready = false;  // set true if DMP init was successful
uint8_t fifoBuffer[64]; // FIFO storage buffer
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
bool IMU_pingpong = false;
String IMU_str;

void setup() {
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    Serial.begin(57600);

    IMU_begin();
}


void loop() {
  if (IMU_Ready) {
    IMU_Yaw();
  }
}
