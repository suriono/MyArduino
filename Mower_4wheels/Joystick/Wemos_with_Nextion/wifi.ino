//String Outstr;

// ============================ Incoming WiFi ================================
void WIFI_incoming() {
   static bool LedOnOff;
   static unsigned long lasttime;
   if (wificlient.available()) {
       myJSON = JSON.parse(wificlient.readStringUntil('<'));
       Serial.println(myJSON);
       if (JSON.typeof(myJSON) != "undefined") {
          if ( myJSON.hasOwnProperty("yaw")) {
             Yaw = (int) myJSON["yaw"];
             //Serial.print("Yaw: "); Serial.print(Yaw); Serial.print(" offset: "); Serial.println(Yaw_offset);
             if (myNex.currentPageId == 1) {
                myNex.writeNum("n0.val", Yaw);
             }
             if (Yaw > 900) { // stop auto orientation
                Yaw_enable = false;
                JSON_send_key_value("YawEnable", 0);
             }
             //Serial.print(" page: "); Serial.println(myNex.currentPageId);
          } else if ( myJSON.hasOwnProperty("yawstatus")) {
             if ( (int) myJSON["yawstatus"] == 0 ) {
                Yaw_enable = false;
             }
          }
       }
       /*
      char ch;
      String instr = "";
      while (wificlient.available()) {
         ch = wificlient.read();
         instr += ch; 
      }
      if ( instr == "OK" ) {
         if ( (millis()-lasttime) > 1000) {
            digitalWrite(LED_BUILTIN, LedOnOff);
            LedOnOff = !LedOnOff;
            lasttime = millis();
         }
      }
      #ifdef DEBUGGING
      //   Serial.print("\t,  Received:"); Serial.println(instr);
      #endif
      */
   }
}

void WIFI_outgoing() {
  static unsigned long lasttime;
  
  if ( (millis() - lasttime) > WIFI_INTERVAL) {
      lasttime = millis();

      if ( Yaw_enable) {  // auto orientation
         if ( millis() > Yaw_expire_time)  {
            Yaw_enable = false;
            JSON_send_key_value("YawEnable", 0);
            Serial.println ("======= Yaw expires ====== ");
         } else {
            JSON_send_key_value("YawGo", Yaw_target);
         }
      } else {
         if (!wificlient.connect(HOST_IP, HOST_PORT)) {
            #ifdef DEBUG
               Serial.println("\t,  Connection to robot failed !!!!");
            #endif
            delay(1);
            return;
         }
         wificlient.print(Joystick_send());
      }
   }
}
