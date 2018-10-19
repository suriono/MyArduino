// ================== Motor Stop ======================

void motorStop() {
  ST.stop();
  #ifdef DEBUG
  //Serial.println("STOP");
  #endif
  last_time = millis();
}

// ================== Set motor speeds ================
void motorRun(int pow_mot, int degree) {

  float max_speed = analogRead(A0) * float(pow_mot) / 1023.0;
  
  float x = cos(degree*PI/180.0);
  float y = sin(degree*PI/180.0);
  float norm_f = abs(max_speed) / max(abs(y-x),abs(y+x));
  int motor1 = round((y-x) * norm_f);
  int motor2 = round(-(y+x) * norm_f);

  #ifdef DEBUG
  Serial.print("Max speed="); Serial.print(max_speed);
  Serial.print(" Power="); Serial.print(pow_mot);
  Serial.print(" Degree="); Serial.print(degree);
  Serial.print(" M1="); Serial.print(motor1); 
  Serial.print(" M2=");  Serial.println(motor2);
  #endif

  ST.motor(1, motor1); delay(15);
  ST.motor(2, motor2);// delay(1);
  //ST.motor(1, motor1); delay(10);
  //ST.motor(2, motor2); delay(10);
}
