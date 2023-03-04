/* currentPageId: shows the id of the current page shown on Nextion
     * WARNING: At the Preinitialize Event of every page, we must write: 
     * printh 23 02 50 xx , where xx the id of the page in hex 
     * (example: for page0, we write: printh 23 02 50 00 , for page9: printh23 02 50 09, for page10: printh 23 02 50 0A) 
     * Use can call it by writing in the .ino file code:  variable = myObject.currentPageId;
     * 
     * lastCurrentPageId: stores the value of the previous page shown on Nextion
     * No need to write anything in Preinitialize Event on Nextion
     * You can call it by writing in the .ino file code:  variable = myObject.lastCurrentPageId;
     *
     * cmdGroup: ONLY for custom commands stores the command group ID 
     *
     * cmdLength: ONLY for custom commands stores the length of the command
     */ 

#define YAW_MAX_POWER 60
#define YAW_MIN_POWER 30
#define YAW_MAX_TIME  10000      // maximum time to do auto alignment
int    Max_Speed_Low = 20;
int    Max_Speed_Med = 35;
int    Max_Speed_Hig = 100;
int    Yaw_offset    = 0;
int    Yaw, Yaw_target;
bool   Yaw_enable = false;
unsigned long Yaw_expire_time;

// ========================== Speed buttons ==================================================
// Preset buttons
void trigger0(){
  Max_Speed = Max_Speed_Low;
  myNex.writeNum("h0.val", Max_Speed);
  myNex.writeNum("n0.val", Max_Speed);
}
void trigger1(){
  Max_Speed = Max_Speed_Med;
  myNex.writeNum("h0.val", Max_Speed);
  myNex.writeNum("n0.val", Max_Speed);
}
void trigger2(){
  Max_Speed = Max_Speed_Hig;
  myNex.writeNum("h0.val", Max_Speed);
  myNex.writeNum("n0.val", Max_Speed);
}
// Change the presets
void trigger3(){         
  Max_Speed_Low = myNex.readNumber("h0.val");;
}
void trigger4(){        
  Max_Speed_Med = myNex.readNumber("h0.val");;
}
void trigger5(){         
  Max_Speed_Hig = myNex.readNumber("h0.val");;
}

// ========================= Verticle Scroll Speed setting=======================================
void trigger6(){
  Max_Speed = myNex.readNumber("h0.val");
  myNex.writeNum("n0.val", Max_Speed);
}

// ======================== page1: Orientations in HEX ==========================================
void trigger16() {      // 11 Hex = 0B: printh 23 02 54 10, Yaw offset
  Yaw_offset = Yaw;
  Yaw_target = 0;        // reset to current location to zero target
  Yaw_enable = false;
  myNex.writeNum("n0.val", 999);
  JSON_send_key_value("YawOffset", 0);
}
void trigger17() {      // 11 Hex = 0B: printh 23 02 54 11, Yaw disable
  Serial.println("Disable auto orientation");
  myNex.writeNum("n0.val", 999);     // to temporarily indicate it's disabled
  Yaw_enable = false;
  JSON_send_key_value("YawEnable", 0);
}
void trigger18() {      // 0 degree
  Serial.println("0-degree");
  Yaw_target = 0;
  Yaw_enable = true;
  Yaw_expire_time = millis() + YAW_MAX_TIME;
  JSON_send_key_value("YawEnable", 1);
  //JSON_send_key_value("YawGo", 0);
}
void trigger19() {      // 180 degree
  Serial.println("180-degree");
  Yaw_target = 180;
  Yaw_enable = true;
  Yaw_expire_time = millis() + YAW_MAX_TIME;
  JSON_send_key_value("YawEnable", 1);
  //JSON_send_key_value("YawGo", 180);
}
void trigger20() {      // -90 degree
  Serial.println("270-degree");
  Yaw_target = 90;
  Yaw_enable = true;
  Yaw_expire_time = millis() + YAW_MAX_TIME;
  //JSON_send_key_value("YawGo", 90);
  JSON_send_key_value("YawEnable", 1);
  //JSON_send_key_value("YawEnable", 1);
}
void trigger21() {      // 90 degree
  Serial.println("90-degree");
  Yaw_target = -90;
  Yaw_enable = true;
  Yaw_expire_time = millis() + YAW_MAX_TIME;
  JSON_send_key_value("YawEnable", 1);
//  JSON_send_key_value("YawGo", -90);
  //JSON_send_key_value("YawEnable", 1);
}
