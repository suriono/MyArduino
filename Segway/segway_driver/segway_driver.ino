
#include <SoftwareSerial.h>
#include <SabertoothSimplified.h>
#include <CurieIMU.h>
#include <MadgwickAHRS.h>

Madgwick filter;
#define IMU_freq     100             // IMU reading frequency (Hz)
float Pitch;                        // Pitch angle
float Pitch_Rate;                   // Pitch angle rate
float Pitch_offset = -2.0;            // Pitch offset angle if no calibration
#define calibrateOffsets true      // turn on calibration
//#define PITCH_OFFSET -2.0           // Pitch offset
#define KP          8.0   // 7 alone=stable, 9=starts to shake a little
#define KP2         1.0   // quadratic term of KP
#define KI          0.1   //0.5  0.1
#define KD          15   //10  20
#define SMOOTH_PD   1.0    // smooth, lower is smoother, not used
// #define RIGHT_COMPENSATION 1.02  // 1.0=no compensation

// July 15: 8,0.2,0,1
//          8, 0.3,0,1

#define DEBUG

#define PIN_TURN_LEFT  8
#define PIN_TURN_RIGHT 4
#define PIN_COMPENSATE_SPEED A1

// Sabertooth parts
#define NOT_A_PIN 0
SoftwareSerial SWSerial(NOT_A_PIN, 3); // RX on no pin (unused), TX on pin 11 (to S1).
SabertoothSimplified ST(SWSerial);     // Use SWSerial as the serial port.

float leftSpeed;
float rightSpeed;
float ctrlwalk = 0;

//int   analogTurn_ref = 500;

// ==============================================

void setup() {
  SWSerial.begin(9600);  // Sabertooth software serial
  SWSerial.flush();
  
  pinMode(PIN_TURN_LEFT,  INPUT_PULLUP);
  pinMode(PIN_TURN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_COMPENSATE_SPEED, INPUT);
  
  Serial.begin(57600);
  ST.stop();               // stop motors

  // start the IMU and filter
  CurieIMU.begin();
  CurieIMU.setGyroRate(IMU_freq);
  CurieIMU.setAccelerometerRate(IMU_freq);
  filter.begin(IMU_freq);

  // Set the accelerometer range to 2G
  CurieIMU.setAccelerometerRange(2);
  // Set the gyroscope range to 250 degrees/second
  CurieIMU.setGyroRange(250);

  // use the code below to calibrate accel/gyro offset values
  if (calibrateOffsets) {
    Pitch_offset = 0.0;  // no fixed offset, use calibration
    
    Serial.println("Internal sensor offsets BEFORE calibration...");
    Serial.print(CurieIMU.getAccelerometerOffset(X_AXIS));
    Serial.print("\t"); // -76
    Serial.print(CurieIMU.getAccelerometerOffset(Y_AXIS));
    Serial.print("\t"); // -235
    Serial.print(CurieIMU.getAccelerometerOffset(Z_AXIS));
    Serial.print("\t"); // 168
    Serial.print(CurieIMU.getGyroOffset(X_AXIS));
    Serial.print("\t"); // 0
    Serial.print(CurieIMU.getGyroOffset(Y_AXIS));
    Serial.print("\t"); // 0
    Serial.println(CurieIMU.getGyroOffset(Z_AXIS));
  //} else {
  
    // To manually configure offset compensation values,
    // use the following methods instead of the autoCalibrate...() methods below
    //CurieIMU.setAccelerometerOffset(X_AXIS, 93.60);
    //CurieIMU.setAccelerometerOffset(Y_AXIS, -42.90);
    //CurieIMU.setAccelerometerOffset(Z_AXIS, -27.30);
    //CurieIMU.setGyroOffset(X_AXIS, 0.0);
    //CurieIMU.setGyroOffset(Y_AXIS, 0.67);
    //CurieIMU.setGyroOffset(Z_AXIS, -0.67);
  //}
    Serial.println("About to calibrate. Make sure your board is stable and upright");
    delay(5000);

    // The board must be resting in a horizontal position for
    // the following calibration procedure to work correctly!
    Serial.print("Starting Gyroscope calibration and enabling offset compensation...");
    CurieIMU.autoCalibrateGyroOffset();
    Serial.println(" Done");

    Serial.print("Starting Acceleration calibration and enabling offset compensation...");
    CurieIMU.autoCalibrateAccelerometerOffset(X_AXIS, 0);
    CurieIMU.autoCalibrateAccelerometerOffset(Y_AXIS, 0);
    CurieIMU.autoCalibrateAccelerometerOffset(Z_AXIS, 1);
    Serial.println(" Done");

    Serial.println("Internal sensor offsets AFTER calibration...");
    Serial.println("X , Y , Z , gyro X, gyro Y, gyro Z");
    Serial.print(CurieIMU.getAccelerometerOffset(X_AXIS));
    Serial.print("\t"); // -76
    Serial.print(CurieIMU.getAccelerometerOffset(Y_AXIS));
    Serial.print("\t"); // -2359
    Serial.print(CurieIMU.getAccelerometerOffset(Z_AXIS));
    Serial.print("\t"); // 1688
    Serial.print(CurieIMU.getGyroOffset(X_AXIS));
    Serial.print("\t"); // 0
    Serial.print(CurieIMU.getGyroOffset(Y_AXIS));
    Serial.print("\t"); // 0
    Serial.println(CurieIMU.getGyroOffset(Z_AXIS));
  }

  Serial.println("Waiting for the segway to level for safety");
  delay(1000);
  //Pitch = 20.0;
  while (!(IMU_Read()) || abs(Pitch) > 5.0) {
    Serial.print("Check initial Pitch: "); Serial.println(Pitch);
    delay(50);
  }
  Motor_Run_Speed(15,15); delay(100);
  Motor_Run_Speed(-15,-15); delay(100);
  ST.stop();
  
  //analogTurn_ref = analogRead(A0);  // reading the reference for turning
  Serial.println("Ready to go");
  delay(1000);
}
// ================== End Setup =================

// ================== Loop ======================

void loop() {
  static unsigned long last_serial_time, last_turn_time, last_comp_speed_time;
  static float ctrlTurn, compensateSpeed = 1.0;
  
  if ( IMU_Read()) { 
    
    if (abs(Pitch) > 20.0) { // prevent disaster
      ST.stop();
    } else {
      PIDSetSpeed();

      leftSpeed-=ctrlTurn;
      rightSpeed *= compensateSpeed;
      rightSpeed+=ctrlTurn;
  
      Motor_Run_Speed(round(leftSpeed), round(rightSpeed));
      last_serial_time = millis();
    }
    ctrlTurn *= 0.95;

    #ifdef DEBUG
     
    //Serial.print(leftSpeed); Serial.print(","); 
    //Serial.print(rightSpeed); Serial.print(",");
    //Serial.println(Pitch);
    #endif
    
  } else if ( (millis() - last_serial_time) > 5000) { // no command for 1 sec
    ST.stop();
  } else if ( (millis() - last_turn_time) > 100) {
      last_turn_time = millis();
      //int turnread = analogRead(A0);
      if ( digitalRead(PIN_TURN_LEFT) == LOW) {
         ctrlTurn = -12.0;
      }
      //} else if (digitalRead(PIN_TURN_RIGHT) == LOW) {
      if (digitalRead(PIN_TURN_RIGHT) == LOW) {
         ctrlTurn = 12.0;
      }
  } else if ( (millis() - last_comp_speed_time) > 1037) { // reading potentiometer
      last_comp_speed_time = millis();
      compensateSpeed = 1.0 + 0.2*(analogRead(PIN_COMPENSATE_SPEED) - 512.0)/1023.0;
      #ifdef DEBUG
        Serial.println(compensateSpeed);
        Serial.println(analogRead(PIN_COMPENSATE_SPEED));
      #endif
  }
}  // ================= End of Loop =====================






// ================= IMU ================================

boolean IMU_Read() {
  int aix, aiy, aiz;
  int gix, giy, giz;
  float ax, ay, az;
  float gx, gy, gz;
  float roll, heading;
  static unsigned long IMU_microsPrevious;
  
  // check if it's time to read data and update the filter
  
  if (micros() - IMU_microsPrevious >= (1000000/IMU_freq)) {
    IMU_microsPrevious = micros();

    // read raw data from CurieIMU
    CurieIMU.readMotionSensor(aix, aiy, aiz, gix, giy, giz);

    // convert from raw data to gravity and degrees/second units
    ax = convertRawAcceleration(aix);
    ay = convertRawAcceleration(aiy);
    az = convertRawAcceleration(aiz);
    gx = convertRawGyro(gix);
    gy = convertRawGyro(giy);
    gz = convertRawGyro(giz);

    // update the filter, which computes orientation
    filter.updateIMU(gx, gy, gz, ax, ay, az);

    // print the heading, pitch and roll
    roll = filter.getRoll();
    Pitch = filter.getPitch() + Pitch_offset;
    heading = filter.getYaw();
    //Serial.print("Orientation: ");
    //Serial.print(heading);
    //Serial.print(" Pitch: ");
    //Serial.print(Pitch);
    //Serial.print(" Roll: ");
    //Serial.println(roll);
    //Serial.print(" gx:"); 
    //Serial.print(gx);
    //Serial.print(","); 
    //Serial.print(gy);
    //Serial.print(","); 
    //Serial.print(gz); Serial.print(",");
    //Pitch_Rate = gy;

    return true;
  }
  return false;
}

float convertRawAcceleration(int aRaw) {
  // since we are using 2G range
  // -2g maps to a raw value of -32768
  // +2g maps to a raw value of 32767
  
  float a = (aRaw * 2.0) / 32768.0;
  return a;
}

float convertRawGyro(int gRaw) {
  // since we are using 250 degrees/seconds range
  // -250 maps to a raw value of -32768
  // +250 maps to a raw value of 32767
  
  float g = (gRaw * 250.0) / 32768.0;
  return g;
}

// ================== End of IMU ====================

// ========================= Motor run general ========================
void Motor_Run_Speed(int pow_mot1, int pow_mot2) { // (-127 to 127)
  ST.motor(1, constrain(pow_mot1,-127,127)); delay(1);
  ST.motor(2, -constrain(pow_mot2,-127,127)); //delay(10)

}

// =================== Self-Balance ==========================
float PID_Balance(float e, float kp, float ki, float kd)
{
  static float es = 0, sum = 0;
  float r;

  sum = constrain(sum+e, -120, 120);
  
  r = kp*e + KP2*e*abs(e) + ki * sum + kd * (e-es); // Pitch_Rate;
  es = e;
  
  //return PID_smooth(r);
  return r;
}
float PID_smooth(float e) {
  static float last_e;
  last_e = e * SMOOTH_PD + (1.0 - SMOOTH_PD) * last_e;
  return last_e;
}
void PIDSetSpeed() // float angle)  // set each motor speed
{
  leftSpeed = rightSpeed = PID_Balance(Pitch+ctrlwalk, KP, KI * 100.0/IMU_freq, KD * IMU_freq / 100.0);
  //ctrlwalk *= 0.9;
}
