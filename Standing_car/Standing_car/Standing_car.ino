#include <SoftwareSerial.h>
#include <SabertoothSimplified.h>

#define DEBUG

#define LBREAK    10      // Left  break
#define RBREAK    4       // Right break 

SoftwareSerial SWSerial(NOT_A_PIN, 9); // RX on no pin (unused), TX on pin 11 (to S1).
SabertoothSimplified ST(SWSerial);     // Use SWSerial as the serial port.


void setup() {
  Serial.begin(57600);
  SWSerial.begin(9600);  // Sabertooth software serial
  SWSerial.flush();

  pinMode(LBREAK, OUTPUT); analogWrite(LBREAK, 250);
  pinMode(RBREAK, OUTPUT); analogWrite(RBREAK, 250);

  Motor_Run_Speed( 30, 30);  
  delay(10000);
  Motor_Run_Speed(0,0);
  analogWrite(LBREAK, 0);  analogWrite(RBREAK, 0);

}

void loop() {
  // put your main code here, to run repeatedly:

}


// ========================= Motor run general ========================
void Motor_Run_Speed(int pow_mot1, int pow_mot2) { // (-127 to 127)
  ST.motor(1, -constrain(pow_mot1,-127,127)); delay(1);
  ST.motor(2, -constrain(pow_mot2,-127,127));
}
