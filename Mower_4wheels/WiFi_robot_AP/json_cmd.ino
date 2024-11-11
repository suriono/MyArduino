#define YAW_MAX_POWER 60
#define YAW_MIN_POWER 20
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
  static unsigned long lasttime;
  static bool LedOnOff;
  int theta_delta, yaw_mag;
  String outstr;
  
  if ( (millis()-lasttime) > 1000) {
     lasttime = millis();
     digitalWrite(LED_BUILTIN, LedOnOff);
     LedOnOff = !LedOnOff;
  }
  
  if ( myJSON.hasOwnProperty("cmd")) {
     String cmdstr = (const char*) myJSON["cmd"];
     // Serial.println( myJSON["cmd"]);
     if ( cmdstr == "motorstop" ) {
        motorStop();
     }
     Yaw_send(); 
  } else {
     
     if ( myJSON.hasOwnProperty("mag")) {
        int amp = (int) myJSON["mag"];
        if ( myJSON.hasOwnProperty("theta")) {
           double theta = (double) myJSON["theta"];
           if ( myJSON.hasOwnProperty("delay")) {
              int motordelay = (int) myJSON["delay"];
              motorRun(amp, theta, motordelay);
           } else {
              motorRun(amp, theta, 10);
           }
        }
        Yaw_send();
     } else if (myJSON.hasOwnProperty("YawOffset") ) {
        Yaw_offset = Yaw + (int) myJSON["YawOffset"];
        Yaw_target = 0;        // reset to current location to zero target
        Yaw_enable = false;
        Yaw_send();
     } else if (myJSON.hasOwnProperty("YawEnable") ) {
        Yaw_enable = (int) myJSON["YawEnable"];
        Yaw_send();
     } else if (myJSON.hasOwnProperty("YawGo") ) {
        Yaw_target = (int) myJSON["YawGo"];
       
        if ( Yaw_enable && IMU_Ready ) {
           IMU_Yaw();    
           
           theta_delta = wrap_Angle( Yaw - Yaw_offset - Yaw_target);

           if ( (theta_delta * last_theta_delta) < 0 ) { // changing direction, prevent oscillation
              PID_I_theta_delta = 0;
           } else if ( abs( wrap_Angle(theta_delta -last_theta_delta)) > 10) { // large spin, reset to prevent oscillation or overshoot
              PID_I_theta_delta = 0;
           } else if (PID_I_theta_delta < 30) {
              PID_I_theta_delta += 1;
           }
           //theta_delta += (Yaw - last_yaw)/2; // anticipate continuous movement
         
           // if (theta_delta < -180) {  // wrap around 360
           //    theta_delta += 360;
           // } else if (theta_delta > 180) {
           //    theta_delta -= 360;
           // }
           // theta_delta = wrap_Angle(theta_delta);
           //Serial.print(" Delta theta: "); Serial.print (theta_delta);

           if ( abs(theta_delta) > 5) {  // tolerate to stop auto orientation
              yaw_mag = map(abs(theta_delta) + PID_I_theta_delta,0,180,YAW_MIN_POWER,YAW_MAX_POWER); // limit by max power
              if (theta_delta > 0) {
                 motorRun(yaw_mag, 90, 100);
              } else {
                 motorRun(yaw_mag, 270, 100);
              }
              Yaw_send();
           } else {
              Yaw_status(0);
              Yaw_enable = false;
              Serial.println("     Orientation reached");
              motorStop();
           }

           last_theta_delta = theta_delta;
        } 
     }
  }
}
