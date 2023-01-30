#include <SoftwareSerial.h>
#include "EasyNextionLibrary.h"  // Include EasyNextionLibrary
//#include <Arduino_JSON.h>
SoftwareSerial mySerial(9, 10); // RX->D1, TX->D2(ESP32)
EasyNex myNex(Serial); // Create an object of EasyNex class with the name < myNex >

//#define DEBUG

//#define SMOOTH_COEF 0.2
//#define TURN_COEF 1.0       // coefficient to slow turn, higher the slower
#define INTERVAL         500  // mS interval of sending data
#define FWDREV_ANALOG    A1
#define LEFTRIGHT_ANALOG A2
//#define SPEED_MAX_ANALOG A3
#define BUTTON_PRESS_PIN 10
int Afwd_ref, Alef_ref, FR_val, LR_val, Mag_raw, Max_Speed;
int Angle_raw, Angle_round;
String Outstr;
//JSONVar myJSON;

// ============================ Setup ===============================

void setup() {
  #ifdef DEBUG
     Serial.begin(57600);
  #endif
  mySerial.begin(57600);
  myNex.begin(9600); // Begin the object with a baud rate of 9600
  
  delay(2000);
  Afwd_ref = analogRead(FWDREV_ANALOG);     // Forward Reverse ref
  Alef_ref = analogRead(LEFTRIGHT_ANALOG);  // Left Right ref
}

// ========================== Loop =================================

void loop() {
  static unsigned long last_time;
  static int interval = INTERVAL;

  Nextion_Loop();

  if ( (millis()-last_time) > interval) {

  
    FR_val = int(analogRead(FWDREV_ANALOG) - Afwd_ref);
    LR_val = int(Alef_ref - analogRead(LEFTRIGHT_ANALOG));
    //SP_val = map(analogRead(SPEED_MAX_ANALOG), 0, 1023, 0, 127);
//    SP_val = 127;  // analogRead(SPEED_MAX_ANALOG);

    // To avoid changing left/right direction due to small values
    // Make it into 45-degree increment
    if (abs(FR_val) < 20) {
      FR_val = 0;
    }
    if (abs(LR_val) < 20) {
      LR_val = 0;
    }

//    Mag_raw   = min(map(max(abs(LR_val),abs(FR_val)), 0, 520, 0, 127), SP_val);
    Mag_raw   = map(max(abs(LR_val),abs(FR_val)), 0, 520, 0, Max_Speed);
    Angle_raw = 360+atan2(LR_val,FR_val)*180.0/PI;

    Angle_raw = int(float((Angle_raw%360) / 45.0) + 0.1) * 45;
    
    //Serial.println(angle_f);

    Outstr  = "{\"mag\":"      + String(Mag_raw);
    Outstr += ",\"theta\":"    + String(Angle_raw);
   // Outstr += ",\"speedmax\":" + String(SP_val);
    Outstr += ",\"delay\":200}<";
    //Serial.println(Outstr);
    
    #ifdef DEBUG
       //Serial.print(FR_val); Serial.print(",");
       //Serial.print(LR_val);
       //Serial.print("Button press="); Serial.print(button_press);
       //Serial.print(", mag=")  ; Serial.print(Mag_raw); 
       Serial.print("Max Speed="); Serial.print(Max_Speed);
       Serial.print(" FR:"); Serial.print(FR_val);
       Serial.print(" LR:"); Serial.print(LR_val);
       Serial.print(" Interval:"); Serial.print( millis() - last_time);
       Serial.println(Outstr);
    #endif
    mySerial.print(Outstr);

    last_time = millis();
     
  }

  interval = INTERVAL;
  if (mySerial.available()) {
    if (mySerial.readStringUntil('<') == "OK") {      
      interval = 250;
    }
  }
}
