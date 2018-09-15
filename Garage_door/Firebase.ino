// ---------------------------------------------------

long Firebase_getResetTime(int new_distance) {
  long resettime = Firebase.getString("garagedoor/resetTime").toInt();
  
  if (resettime > 1999) {  // a success
    if (last_resettime > 0) {  // not the first time
      if (resettime > last_resettime) {  // new reset time
        Firebase.remove("garagedoor/data/"); // remove data
        if (Firebase.success()) {
          last_resettime = resettime;
          Firebase_Send_Distance(new_distance);
        }
      }
    } else { // first time
      last_resettime = resettime;
    }
  }
  Serial.print("Reset time read: "); Serial.println(resettime);
  //if (Firebase.failed()) {
  //  Firebase_fail("Reset Fail");
  //  return 0;
  //}
  return resettime;
}

// ------------------------------------------------------------
unsigned long get_Server_Time();

bool Firebase_Send_Distance(int new_distance) {
  unsigned long server_epoch = get_Server_Time();
  long time_from_reset;
  
  if (server_epoch > 0) {
    last_server_epoch = server_epoch;
    last_server_epoch_elapsed = millis();
  } else {
    server_epoch = last_server_epoch + (millis() - last_server_epoch_elapsed) / 1000;
  }
  
  time_from_reset = server_epoch - last_resettime; //  resettime;
  Serial.print("Distance time from reset: "); Serial.println(time_from_reset);

  if ( time_from_reset > 10000000 || time_from_reset < 0 ) return false; // if something wrong
      
  String sendstr = "dist:" + String(new_distance) + "&time:" + String(time_from_reset);
  Firebase.pushString("garagedoor/data/", sendstr);
  //Serial.println("==== Updating Firebase");
  //if (Firebase.failed()) {
     Firebase_fail("Send_Distance");
  //   return false;
  //}
 
  time_firebase_update = millis();
  return true;
}


// -------------------------------------------------------
void Firebase_fail(String function_name) {
   Serial.print("==== Firebase failed: ");
   Serial.println(function_name);
   //Serial.println(Firebase.error());  

   // resetFunc(); //call reset 
   // Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

// --------------------------------------------------
int Firebase_Door_Pin_Number(int);

bool Firebase_getPushButtonRemote() {  // whether to push the wall remote button to open/close
  int wall_pin = Firebase.getString("garagedoor/pushButtonWallRemote").toInt();
  //Serial.print("garage door pin read: "); Serial.println(wall_pin);
  //digitalWrite(RELAY_OPENER, LOW);

  if ( wall_pin > 1) Firebase.setString("garagedoor/pushButtonWallRemote", "1");
 
  int reset_wall_pin = Firebase.getString("garagedoor/pushButtonWallRemote").toInt();
  if ( reset_wall_pin == 1) { // make sure it has been reset so it does not open/close continuously
     if (Firebase_Door_Pin_Number(wall_pin)) {  // pin number to open the garage
         digitalWrite(RELAY_OPENER, HIGH);
         delay(500);
         digitalWrite(RELAY_OPENER, LOW);
         Firebase.setString("garagedoor/pushButtonWallRemote", "1");
        
      }
  }
  return true;
}

// ------------------------------------------------------
void Firebase_debug(String debug_msg) {
  Firebase.setString("garagedoor/debug/", debug_msg);
}


void(* resetFunc) (void) = 0;//declare reset function at address 0

// ============= End of Firebase ================================
