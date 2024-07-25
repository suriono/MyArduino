
// ===================== process_JSON =====================
void process_JSON() {
  if ( myJSON.hasOwnProperty("Run")) {
     int direct = (int) myJSON["Run"];
     
     if (direct == 1) {  // forward
        #ifdef DEBUGGING
           Serial.println("     forwawrd....");
        #endif
        Motor_Forward(100, 10);
     } else if (direct == 2) {  // forward
        Serial.println("     reverse....");
        Motor_Reverse(100, 10);
     } else if (direct == 3) {  // forward
        Serial.println("     right ....");
        Motor_Right(100, 10);
      } else if (direct == 4) {  // forward
        Serial.println("     left ....");
        Motor_Left(100, 10);
      } else if (direct == 5) {  // forward
        Serial.println("     mowing ....");
        Motor_Mow(100, 10);
     } else if (direct == 0) {
        Serial.print(" STOP");
        Motor_Stop();
     }
  }
  wificlient.print("OK<");
  wificlient.flush();
}
