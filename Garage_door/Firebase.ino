// ------------------------------------------------
bool Firebase_Send_Distance(int new_distance) {
  long resettime = Firebase_getResetTime();
  unsigned long server_epoch = get_Server_Time();
  long new_time;
  static long delta_last_time_distance;
  static boolean isFirstFail = true;
  
  if (server_epoch < 1) { // try again
      new_time = delta_last_time_distance + last_time_distance / 1000;
      Serial.println("Zero Distance Server Epoch =====");
      if (isFirstFail) return false;
  } else {
      new_time = server_epoch - resettime;
      delta_last_time_distance = new_time - last_time_distance / 1000;
      isFirstFail = false;
  }
  Serial.print("Firebase time from reset: "); Serial.println(new_time);

  if ( new_time > 10000000 ) return false; // if something wrong
      
  String sendstr = "dist:" + String(new_distance) + "&time:" + String(new_time);
  Firebase.pushString("garagedoor/data/", sendstr);
  //Serial.println("==== Updating Firebase");
  if (Firebase.failed()) {
     Firebase_fail();
     return false;
  }
 
  time_firebase_update = millis();
  return true;
}


// -------------------------------------------------------
void Firebase_fail() {
   Serial.print("==== Firebase failed: ");
   Serial.println(Firebase.error());  

   // resetFunc(); //call reset 
   // Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

// --------------------------------------------------
bool Firebase_getPushButtonRemote() {  // whether to push the wall remote button to open/close
  int wall_pin = Firebase.getString("garagedoor/pushButtonWallRemote").toInt();
  digitalWrite(RELAY_OPENER, LOW);
  
  Firebase.setString("garagedoor/pushButtonWallRemote", "1");
  if (Firebase.failed()) {
    return false;
  }
  int reset_wall_pin = Firebase.getString("garagedoor/pushButtonWallRemote").toInt();
  if ( reset_wall_pin == 1) { // make sure it has been reset so it does not open/close continuously
    if (wall_pin == WALL_PIN) {  // pin number to open the garage
      digitalWrite(RELAY_OPENER, HIGH);
      delay(500);
      digitalWrite(RELAY_OPENER, LOW);
    } else {
      return false;
    }
  } else {
    return false;
  }
  return true;
}
// ---------------------------------------------------
long Firebase_getResetTime() {
  long resettime = Firebase.getString("garagedoor/resetTime").toInt();

  //Serial.print("reset time = "); Serial.println(Firebase.getString("garagedoor/resetTime"));
  if (Firebase.failed()) {
      Firebase_fail();
      return 0;
  }
  return resettime;
}

// ------------------------------------------------------
void Firebase_debug(String debug_msg) {
  Firebase.setString("garagedoor/debug/", debug_msg);
}


void(* resetFunc) (void) = 0;//declare reset function at address 0

// ============= End of Firebase ================================


