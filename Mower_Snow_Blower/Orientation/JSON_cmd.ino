#define YAW_MAX_POWER 20
#define YAW_MIN_POWER 10
#define YAW_MAX_TIME  10000      // maximum time to do auto alignment
int    Max_Speed_Low = 20;
int    Max_Speed_Med = 35;
int    Max_Speed_Hig = 100;
int    Yaw_offset    = 0;
int    Yaw_target;
bool   Yaw_enable = false;
unsigned long Yaw_expire_time;
int last_theta_delta  = 0;
int PID_I_theta_delta = 0;

// ==================== -180 < angle < 180 ================
int wrap_Angle(int nag) {
  int ag = nag;
  if (ag < -180) {  // wrap around 360
     ag += 360;
  } else if (ag > 180) {
     ag -= 360;
  }
  return ag;
}

// ===================== process_JSON =====================
void process_JSON() {
//  static unsigned long lasttime;
//  static bool LedOnOff;
 // int theta_delta, yaw_mag;
  //String outstr;
  
  if ( myJSON.hasOwnProperty("cmd")) {
     String cmdstr = (const char*) myJSON["cmd"];
     // Serial.println( myJSON["cmd"]);
     if ( cmdstr == "getyaw" ) {
    //    motorStop();
        Yaw_send();
     }
  } else {
     if (myJSON.hasOwnProperty("YawOffset") ) {
        Yaw_offset = Yaw + (int) myJSON["YawOffset"];
       // Yaw_target = 0;        // reset to current location to zero target
    //    Yaw_enable = false;
        Yaw_send();
     } 
  }
}
