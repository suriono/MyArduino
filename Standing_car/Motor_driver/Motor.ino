#define PID_KP 0.1
#define PID_KI 0.000
#define PID_KD 0.01

float rightspeedsensor;


// ========================= Motor run general ========================
void Motor_Run_Speed(int pow_mot1, int pow_mot2) { // (-127 to 127)
   ST.motor(1, -constrain(pow_mot1,-127,127)); delay(1);
   ST.motor(2, -constrain(pow_mot2,-127,127));
}

// ========================= Motor & Break ===========================
void Motor_Run(int leftmot, int rightmot) {
  
   if (leftmot < 0) {
      leftmot = leftmot / 2;
   }
   if (rightmot < 0) {
      rightmot = rightmot / 2;
   }

   Motor_Run_Speed(leftmot, rightmot);
   
   Serial.print(leftmot); Serial.print(" , "); Serial.println(rightmot); 
}  

// ========================= Motor run uses sensor and PID =============

void Motor_PID(int leftPot, int rightPot) { 
  static float sumRight_pid = 0.0; 
  static float lastspeed = 0.0;
  int totalRightDiff = 0;
  static unsigned long lastFullPower = millis();

  // if in reverse
  if (leftPot < 0) {
     leftPot = leftPot / 2;
  }
  if (rightPot < 0) {
     rightPot = rightPot / 2;
  }

   
   if ( abs(leftPot-rightPot) < 10) { // when it's not turning

      float diffRight = (rightPot - rightspeedsensor*signInt(rightPot)); 

/*
      sumRight_pid += diffRight * PID_KI;
      if ( abs(sumRight_pid) > abs(rightPot/4)) {
         sumRight_pid = rightPot / 2;
      }

      totalRightDiff = int(diffRight * PID_KP + sumRight_pid + PID_KD*(lastspeed-rightspeedsensor));
      */
      totalRightDiff = int(diffRight * PID_KP);

     // if (rightspeedsensor < 20 || abs(rightPot) < MAX_SPEED - 10) {
     //    lastFullPower = millis();
     // }

      //if (rightspeed > 30) {
       // if ( (millis() - lastFullPower) > 2000) { // after two seconds full speed
      //     totalRightDiff = 20; // add 10 to the top speed
      //  }
      //}
   }
   Serial.print(rightspeedsensor); Serial.print(","); Serial.println(rightPot);
   Motor_Run_Speed(leftPot + totalRightDiff, rightPot + totalRightDiff);

   
  
}

// ======== retun the sign of an integer ==================

int signInt(int value) { 
  return ((value>0)-(value<0)); 
}

