
// ===================== process_JSON =====================
void process_JSON() {
  if ( myJSON.hasOwnProperty("Run")) {
     int direct = (int) myJSON["Run"];
     
     if (direct == 1) {  // forward
        Serial.println("     forwawrd....");
        Motor_Forward(100, 10);
     } else if (direct == 2) {  // forward
        Serial.println("     reverse....");
        Motor_Reverse(100, 10);
     } else if (direct == 0) {
        Serial.println("          STOP");
        Motor_Stop();
     }
  }
  wificlient.print("OK<");
  wificlient.flush();
}
