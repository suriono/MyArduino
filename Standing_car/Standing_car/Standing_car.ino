#include <SoftwareSerial.h>
#include <SabertoothSimplified.h>

#define DEBUG

#define LBREAKPIN    10      // Left  break
#define RBREAKPIN    4       // Right break 
#define STIRPIN      A0      // potentiometer pin to read stirring

SoftwareSerial SWSerial(NOT_A_PIN, 9); // RX on no pin (unused), TX on pin 11 (to S1).
SabertoothSimplified ST(SWSerial);     // Use SWSerial as the serial port.

int Stir_Ref;                 // Reference reading of the stirring potentiomenter

void setup() {
  Serial.begin(57600);
  SWSerial.begin(9600);  // Sabertooth software serial
  SWSerial.flush();

  pinMode(LBREAKPIN, OUTPUT); 
  pinMode(RBREAKPIN, OUTPUT);
  pinMode(STIRPIN, INPUT);

  Break_Run(250 , 250);
  Motor_Run_Speed( 30, 30);  
  delay(10000);
  Motor_Run_Speed(0,0);
  Break_Run(0 , 0);
  //analogWrite(LBREAKPIN, 0);  analogWrite(RBREAKPIN, 0);

   Stir_Ref = analogRead(STIRPIN);
}

void loop() {
  int stir_read = analogRead(STIRPIN);
  int stir_delta = stir_read - Stir_Ref;
  int speed;
  
  Serial.print(stir_read); Serial.print(" , "); Serial.println(stir_delta);
  
  //Motor_Break_Run(speed + stir_delta, speed - stir_delta);
  
  delay(1000);

}

// ========================= Motor & Break ===========================
void Motor_Break_Run(int leftmot, int rightmot) {
   Motor_Run_Speed(leftmot, rightmot);
   Break_Run(abs(leftmot*2) , abs(rightmot*2));
}  

// ========================= Motor run general ========================
void Motor_Run_Speed(int pow_mot1, int pow_mot2) { // (-127 to 127)
   ST.motor(1, -constrain(pow_mot1,-127,127)); delay(1);
   ST.motor(2, -constrain(pow_mot2,-127,127));
}

// ========================= Breaks ===================================
void Break_Run(int leftmot, int rightmot) {
   analogWrite(LBREAKPIN, leftmot);  analogWrite(RBREAKPIN, rightmot);
}
