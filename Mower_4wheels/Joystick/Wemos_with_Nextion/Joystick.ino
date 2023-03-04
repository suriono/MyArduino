
void Joystick_get() {
   if (digitalRead(D3)) { 
      FR_val = analogRead(A0);
      digitalWrite(D3, LOW);
   } else {
       LR_val = analogRead(A0);
       digitalWrite(D3, HIGH);
   }
}

// ================= Joystick send command =======================

String Joystick_send() {
   String Outstr;
  
   Joystick_get();
   int FR_ref = FR_val - Afwd_ref;
   int LR_ref = Alef_ref - LR_val;

   #ifdef DEBUGGING
      //  Serial.print(FR_val); Serial.print(",");Serial.print(LR_val);
      // Serial.print("  ");Serial.print(FR_ref); Serial.print(",");Serial.println(LR_ref);
   #endif
   // To avoid changing left/right direction due to small values
   // Make it into 45-degree increment
   if (abs(FR_ref) < 20) {
      FR_ref = 0;
   }
   if (abs(LR_ref) < 20) {
      LR_ref = 0;
   }

   Mag_raw   = map(max(abs(LR_ref),abs(FR_ref)), 0, 520, 0, Max_Speed);
      
   Angle_raw = 360+atan2(LR_ref,FR_ref)*180.0/PI;
   Angle_raw = int(float((Angle_raw%360) / 45.0) + 0.1) * 45; // round to 45-deg

   Outstr  = "{\"mag\":"      + String(Mag_raw);
   Outstr += ",\"theta\":"    + String(Angle_raw);
   // Outstr += ",\"speedmax\":" + String(SP_val);
   Outstr += ",\"delay\":10}<";
   return Outstr;
}
