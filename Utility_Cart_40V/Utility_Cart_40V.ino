#include <SabertoothSimplified.h>
#include <SoftwareSerial.h>
#include <Encoder.h>

#define SPEED_PIN               A0     // Potentiometer foot pedal
#define BRAKE_PIN               A1     // Potentiometer hand brake
#define BRAKE_MAX               800    // analog max value
#define SABERTOOTH_S1           5      // Sabertooth S1 input pin

SoftwareSerial       SerialRobot(4 , SABERTOOTH_S1);     // RX, TX
SabertoothSimplified ST(SerialRobot);
Encoder              myEnc(2, 3);

int Speed_Init_Analog, Brake_Init_Analog;
long oldPosition = -999;                // encoder position
long newPosition, speedOldPosition;
double speed_count;
double mph;

// ================= Foot pedal to Sabertooh ========

int FootPedal_Value() {
  int constrainvalue = constrain(analogRead(SPEED_PIN),Speed_Init_Analog, 900);
  
  int stval = map(constrainvalue, Speed_Init_Analog,900,0,127);
  //Serial.print(constrainvalue);Serial.print(",");Serial.print(stval);
  Serial.print(stval); Serial.print("\t");
  return stval;
}

// ================= Hand brake to Sabertooh ========

int HandBrake_Value() {
  int constrainvalue = constrain(analogRead(BRAKE_PIN),Brake_Init_Analog, BRAKE_MAX);
  
  int stval = map(constrainvalue, Brake_Init_Analog,BRAKE_MAX,0,-127);
  //Serial.print(","); Serial.print(constrainvalue);
  Serial.print("\t");Serial.print(stval);
  return stval;
}

// ===================== SETUP ==============================

void setup() {
    Serial.begin(57600);
    SerialRobot.begin(9600);

    ST.stop();
    Speed_Init_Analog = analogRead(SPEED_PIN); // reference speed read(foot pedal)
    Brake_Init_Analog = analogRead(BRAKE_PIN); // reference brake read(handle)
}

void loop() {
  static int prev_stval = 0;
  static unsigned long last_time_motor;
  int stval;
  unsigned long elapsed_time = millis() - last_time_motor;

  if ( elapsed_time > 200)  {   // moving motor
    last_time_motor = millis();
   
    int fwdval   = FootPedal_Value();
    int brakeval = HandBrake_Value();

    if (brakeval < -4 && mph > 0.4) {   // braking, overwrite forward and if currently forward
      if (mph > 10.0) {                 // fast enough to direct brake
        stval = brakeval;
      } else {                            // slower, proportional to speed
        stval = int(double(brakeval) * mph / 10.0); 
      }
    } else {                            // forward
      stval = fwdval;
    }
    if (prev_stval == 0 && abs(stval) > 20) { // unexpected jump value when stopping
      Serial.println("   ==== detect jump====");
      prev_stval = stval;
      stval = 0;
    } else {
      prev_stval = stval;
    }
 
    ST.motor(1, stval);

    mph = 1000.0*(speedOldPosition - newPosition)*2.237/double(0.83*elapsed_time*5760); // m/s
    
    Serial.print("\t"); Serial.print(newPosition);
    Serial.print("\t"); Serial.println(mph);

    speedOldPosition = newPosition;
    
  }

  newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    
  }
}
