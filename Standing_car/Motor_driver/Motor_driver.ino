#include <SoftwareSerial.h>
#include <SabertoothSimplified.h>

#define DEBUG

#define STIRPIN      A0      // potentiometer pin to read stirring
#define SPEEDPIN     A1      // potentiometer pin to read speed

#define MAX_SPEED    40

SoftwareSerial SaberSerial(NOT_A_PIN, 9); // RX on no pin (unused), TX on pin 11 (to S1).
SabertoothSimplified ST(SaberSerial);     // Use SaberSerial as the serial port.
SoftwareSerial SpeedSensorSerial(4, NOT_A_PIN); // RX on no pin

int Stir_Ref;                 // Reference reading of the stirring potentiomenter
int Speed_Ref;                 // Reference reading of the stirring potentiomenter
//volatile unsigned int right_speed_counter = 0;   // right motor speed encoder counter

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
  static unsigned long last_encoder_time = millis();
  
  if ( (millis() - last_loop_time) > 200 ) {    // only read the potentiometers periodically
    last_loop_time = millis();                // reset the timer loop
  
    stir_read  = (analogRead(STIRPIN) - Stir_Ref)/2;
    speed_read = map(analogRead(SPEEDPIN) - Speed_Ref, -512, 512, -MAX_SPEED,MAX_SPEED);

  }
  
  if (SpeedSensorSerial.available() > 0) { // read encoder speed sensor
  
      getSpeedSensor(speed_read-stir_read);    // read speed sensor

      if ( (millis() - last_encoder_time) < 100 ) { // make sure encoder is read
        Motor_PID(speed_read + stir_read, speed_read - stir_read);
        //Motor_Run(speed_read + stir_read, speed_read - stir_read);  // move down later
      }

      last_encoder_time = millis();   // last reading encoder
  }
}




