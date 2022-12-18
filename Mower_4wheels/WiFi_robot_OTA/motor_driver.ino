// ================== Motor Stop ======================

void motorStop() {
  ST.stop();
  //last_motor_run = millis();
  #ifdef DEBUG
    Serial.println("Motor Stop");
  #endif
}

// ================== Set motor speeds ================
void motorRun(int pow_mot, int degree, unsigned long delaytime) {

//  last_motor_run = millis();
  
  // float max_speed = 300.0 * float(pow_mot) / 1023.0; // analogRead(A0) * float(pow_mot) / 1023.0;
  float max_speed = float(pow_mot) ; // analogRead(A0) * float(pow_mot) / 1023.0;

  degree = (degree + 360) % 360;
  if (degree > 90 && degree < 180) {
    degree = 360 - degree; //  + 180 - 2*(degree - 90);
  } else if (degree > 180 && degree < 270) {
    degree = 360 - degree;
  }
  
  float y = cos(-degree*PI/180.0);
  float x = sin(degree*PI/180.0);
  float norm_f = abs(max_speed) / max(abs(y-x),abs(y+x));
  //int motor1 = round(-(y-x) * norm_f);
  int motor1 = round(-(y-x) * norm_f);
  int motor2 = round(-(y+x) * norm_f);

  #ifdef DEBUG
    
    Serial.print(" X="); Serial.print(String(x));
    Serial.print(" Y="); Serial.print(String(y));
    Serial.print(" Max speed="); Serial.print(max_speed);
    Serial.print(" Power="); Serial.print(pow_mot);
    Serial.print(" Degree="); Serial.print(degree);
    Serial.print(" M1="); Serial.print(motor1); 
    Serial.print(" M2=");  Serial.print(motor2);
    Serial.println();
  #endif

  ST.motor(1, motor1); delay(15);
  ST.motor(2, motor2);
  delay(delaytime);
}
