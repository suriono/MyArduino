#include <SoftwareSerial.h>
#include "EasyNextionLibrary.h"  // Include EasyNextionLibrary
//#include <Arduino_JSON.h>
SoftwareSerial mySerial(9, 10); // RX->D1, TX->D2(ESP32)
EasyNex myNex(Serial); // Create an object of EasyNex class with the name < myNex >

//#define DEBUGGING

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
unsigned long last_sent;

// ============================ Setup ===============================

void setup() {
  #ifdef DEBUGGING
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
  //static unsigned long last_time;
  static int interval = INTERVAL;

  Nextion_Loop();

  if ( (millis()-last_sent) > interval) {

  
    FR_val = int(analogRead(FWDREV_ANALOG) - Afwd_ref);
    LR_val = int(Alef_ref - analogRead(LEFTRIGHT_ANALOG));
    
    // To avoid changing left/right direction due to small values
    // Make it into 45-degree increment
    if (abs(FR_val) < 20) {
      FR_val = 0;
    }
    if (abs(LR_val) < 20) {
      LR_val = 0;
    }

    Mag_raw   = map(max(abs(LR_val),abs(FR_val)), 0, 520, 0, Max_Speed);
    Angle_raw = 360+atan2(LR_val,FR_val)*180.0/PI;

    Angle_raw = int(float((Angle_raw%360) / 45.0) + 0.1) * 45;
    
    //Serial.println(angle_f);

    Outstr  = "{\"mag\":"      + String(Mag_raw);
    Outstr += ",\"theta\":"    + String(Angle_raw);
   // Outstr += ",\"speedmax\":" + String(SP_val);
    Outstr += ",\"delay\":200}<";
    //Serial.println(Outstr);
    
    mySerial.print(Outstr);

    //last_time = millis();
     
  }

  interval = INTERVAL;
  if (mySerial.available()) {
     //myJSON = JSON.parse(mySerial.readStringUntil('<'));
     String Instr = mySerial.readStringUntil('<');
     int commaindex = Instr.indexOf(',');
     float yaw   = Instr.substring(0,commaindex).toFloat();
     int elapsed = Instr.substring(commaindex+1).toInt();
     last_sent = millis();
  
  }
}
