#include <SoftwareSerial.h>
#include <SabertoothSimplified.h>

#define DEBUG

#define STIRPIN      A0      // potentiometer pin to read stirring
#define SPEEDPIN     A1      // potentiometer pin to read speed

#define MAX_SPEED    30
//#define SPEED_SMOOTH 0.1

SoftwareSerial SaberSerial(NOT_A_PIN, 9); // RX on no pin (unused), TX on pin 11 (to S1).
SabertoothSimplified ST(SaberSerial);     // Use SaberSerial as the serial port.
SoftwareSerial SpeedSensorSerial(4, NOT_A_PIN); // RX on no pin


int Stir_Ref;                 // Reference reading of the stirring potentiomenter
int Speed_Ref;                 // Reference reading of the stirring potentiomenter
volatile unsigned int right_speed_counter = 0;   // right motor speed encoder counter

// ========================== SETUP ==============================================

void setup() {
  Serial.begin(57600);
  SaberSerial.begin(9600);  // Sabertooth software serial
  SpeedSensorSerial.begin(57600);
  SaberSerial.flush();
  
  pinMode(STIRPIN, INPUT);
  pinMode(SPEEDPIN, INPUT);
  
  while ( abs( analogRead(STIRPIN) - 540) > 30 ) { // ensure it's center
  }
  while ( abs( analogRead(SPEEDPIN) - 515) > 30 ) { // ensure it's center
  }
  Stir_Ref = analogRead(STIRPIN);
  Speed_Ref = analogRead(SPEEDPIN);
}

void loop() {
  static unsigned long last_loop_time = millis();
  static int stir_read;
  static int speed_read;
  static unsigned long last_right_enc_time = millis();
  
  if ( (millis() - last_loop_time) > 200 ) {    // only read the potentiometers periodically
    last_loop_time = millis();                // reset the timer loop
  
    stir_read  = (analogRead(STIRPIN) - Stir_Ref)/2;
    speed_read = map(analogRead(SPEEDPIN) - Speed_Ref, -512, 512, -MAX_SPEED,MAX_SPEED);

   
  }
  
  if (SpeedSensorSerial.available() > 0) {
      float rightspeedsensor = SpeedSensorSerial.parseFloat();
      Serial.print(rightspeedsensor); Serial.print(",");
      Motor_Break_Run(speed_read + stir_read, speed_read - stir_read);  // move down later
  }
}

// ========================= Motor & Break ===========================
void Motor_Break_Run(int leftmot, int rightmot) {
  
   if (leftmot < 0) {
      leftmot = leftmot / 2;
   }
   if (rightmot < 0) {
      rightmot = rightmot / 2;
   }

   Motor_Run_Speed(leftmot, rightmot);
   
   Serial.print(leftmot); Serial.print(" , "); Serial.println(rightmot); 
}  

// ========================= Motor run general ========================
void Motor_Run_Speed(int pow_mot1, int pow_mot2) { // (-127 to 127)
   ST.motor(1, -constrain(pow_mot1,-127,127)); delay(1);
   ST.motor(2, -constrain(pow_mot2,-127,127));
}

