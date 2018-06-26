#include <SoftwareSerial.h>
#include <SabertoothSimplified.h>

#define DEBUG

#define LBREAKPIN    10      // Left  break
#define RBREAKPIN    4       // Right break 
#define STIRPIN      A0      // potentiometer pin to read stirring
#define SPEEDPIN     A1      // potentiometer pin to read speed

#define MAX_SPEED    30
#define SPEED_SMOOTH 0.1
#define BREAK_M      1.0     // Break multiplier
#define BREAK_SMOOTH 0.001     // Break smooth parameter, lower for smoother

SoftwareSerial SWSerial(NOT_A_PIN, 9); // RX on no pin (unused), TX on pin 11 (to S1).
SabertoothSimplified ST(SWSerial);     // Use SWSerial as the serial port.

int Stir_Ref;                 // Reference reading of the stirring potentiomenter
int Speed_Ref;                 // Reference reading of the stirring potentiomenter

void setup() {
  Serial.begin(57600);
  SWSerial.begin(9600);  // Sabertooth software serial
  SWSerial.flush();

  pinMode(LBREAKPIN, OUTPUT); 
  pinMode(RBREAKPIN, OUTPUT);
  pinMode(STIRPIN, INPUT);
  pinMode(SPEEDPIN, INPUT);

  /*
  Break_Run(250 , 250);
  Motor_Run_Speed( 30, 30);  
  delay(5000);
  Motor_Run_Speed(0,0);
  Break_Run(0 , 0);
  */
  
  while ( abs( analogRead(STIRPIN) - 540) > 30 ) { // ensure it's center
  }
  while ( abs( analogRead(SPEEDPIN) - 515) > 30 ) { // ensure it's center
  }
  Stir_Ref = analogRead(STIRPIN);
  Speed_Ref = analogRead(SPEEDPIN);
}

void loop() {
  int stir_read  = (analogRead(STIRPIN) - Stir_Ref)/4;
  int speed_read = map(analogRead(SPEEDPIN) - Speed_Ref, -512, 512, -MAX_SPEED,MAX_SPEED);

  //Serial.print(speed_read); Serial.print(" , ");
  //Serial.println(stir_read);
  
  Motor_Break_Run(speed_read + stir_read, speed_read - stir_read);
  
  delay(1);

}

// ========================= Motor & Break ===========================
void Motor_Break_Run(int leftmot, int rightmot) {
   static float lastleftbreak;
   static float lastrightbreak;
   static float lastleftmot;
   static float lastrightmot;
   static unsigned long laststoptime;
   float leftbreak = abs(leftmot*BREAK_M);
   float rightbreak = abs(rightmot*BREAK_M);

   // motor part
   if (leftmot < 0) {
      leftmot = leftmot / 2;
   }
   if (rightmot < 0) {
      rightmot = rightmot / 2;
   }
/*
   lastleftbreak  = BREAK_SMOOTH * leftbreak + (1.0-BREAK_SMOOTH)*lastleftbreak;
   lastrightbreak = BREAK_SMOOTH * rightbreak + (1.0-BREAK_SMOOTH)*lastrightbreak;

   lastleftmot  = SPEED_SMOOTH * leftmot  + (1.0-SPEED_SMOOTH)*lastleftmot;
   lastrightmot = SPEED_SMOOTH * rightmot + (1.0-SPEED_SMOOTH)*lastrightmot;

   
   if (abs(leftmot) < 5 && abs(rightmot) < 5) {
      if ( (millis() - laststoptime) > 1000 ) {
         lastleftbreak  = 0;
         lastrightbreak = 0;
         lastleftmot  = 0;
         lastrightmot = 0;
         laststoptime = millis();
      }
   }
   


   
   Motor_Run_Speed(lastleftmot, lastrightmot);
   Break_Run(int(lastleftbreak) , int(lastrightbreak));

   Serial.print(lastleftmot); Serial.print(" , "); Serial.print(lastrightmot); Serial.print(" , ");
   Serial.print(lastleftbreak); Serial.print(" , "); Serial.println(lastrightbreak);
   */
   Motor_Run_Speed(leftmot, rightmot);
   //Break_Run(abs(leftmot) , abs(rightmot));
   Break_Run(250,250);
   Serial.print(leftmot); Serial.print(" , "); Serial.println(rightmot); 
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
