#define PID_KP 0.2
#define PID_KI 0.000
#define PID_KD 0.01
#define SPEED_SMOOTH 0.5  // was 0.03

float leftspeedsensor;
float rightspeedsensor;

// ========================= Motor run general ========================
void Motor_Run_Speed(int pow_mot1, int pow_mot2) { // (-127 to 127)
   ST.motor(1, -constrain(pow_mot1,-127,127)); delay(1);
   ST.motor(2, -constrain(pow_mot2,-127,127));
}

// ========================= Motor run uses sensor and PID =============

void Motor_PID(int leftPot, int rightPot) { 
  
  static float lastRightSpeed = 0.0;
  static float lastLeftSpeed = 0.0;
  int totalLeftDiff = 0;
  int totalRightDiff = 0;
  static unsigned long lastStopTime = millis();

  if ( (abs(leftPot) + abs(rightPot)) < 4 ) { // when it stops
     lastStopTime = millis();
  }

  if ( (millis() - lastStopTime) < 1500) { // first 2 sec after stop, go easy
     leftPot = leftPot / 2;
     rightPot = rightPot / 2;
  } else { // if in reverse
     if (leftPot < 0) {
        leftPot = leftPot / 2;
     }
     if (rightPot < 0) {
        rightPot = rightPot / 2;
     }
  }

   
   
      float diffLeft  = (leftPot  - leftspeedsensor *signInt(leftPot));
      float diffRight = (rightPot - rightspeedsensor*signInt(rightPot)); 

      totalLeftDiff  = int(diffLeft  * PID_KP);
      totalRightDiff = int(diffRight * PID_KP);

   float Speed_Smooth = SPEED_SMOOTH;  // smoothing factor
   //if ( abs(leftPot-rightPot) > 10) {  // when it's turning
   //   Speed_Smooth *= 2.0;              
   //}

   // Smoothing
   float totalLeft  = (leftPot  + totalLeftDiff) * Speed_Smooth + (1.0-Speed_Smooth)* lastLeftSpeed;
   float totalRight = (rightPot + totalRightDiff)* Speed_Smooth + (1.0-Speed_Smooth)* lastRightSpeed;
   
   lastRightSpeed = totalRight;
   lastLeftSpeed  = totalLeft;

   Serial.print(leftspeedsensor); Serial.print(",");
   Serial.print(rightspeedsensor); Serial.print(","); 
   Serial.print(leftPot); Serial.print(","); Serial.println(rightPot);
   Motor_Run_Speed(int(totalLeft), int(totalRight));

   
  
}

// ======== retun the sign of an integer ==================

int signInt(int value) { 
  return ((value>0)-(value<0)); 
}

