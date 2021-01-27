// ---------------------------------------------------

long Firebase_getResetTime(int new_distance, String inpstr) {
  long resettime;
  //delay(1000);
  yield();
  if (Firebase.getString(firebaseData, "garagedoor/resetTime")) {
    resettime = firebaseData.stringData().toInt();
  }
  
  //long resettime = Firebase.getString("garagedoor/resetTime").toInt();
  yield();
  delay(1000);
  
  Firebase_debug("Reset@" + String(resettime) + inpstr);
  delay(1000);

  if (resettime > 1999) {  // a success
    //Serial.print("Read data: "); Serial.print(Firebase.getString("garagedoor/data/"));
    //Serial.println("====");
    if (last_resettime > 0) {  // not the first time
      if (resettime > last_resettime) {  // new reset time
        if (Firebase.deleteNode(firebaseData, "garagedoor/data/")) { // remove data
          delay(1000);
          Firebase_debug("fail-remove_data");
        
        //if (Firebase.success()) {
          delay(1000);
          Firebase_debug("success-remove-data");
          last_resettime = resettime;
          delay(1000);
          Firebase_Send_Distance(new_distance);
        } else {
          delay(1000);
          if(Firebase.deleteNode(firebaseData, "garagedoor/data/")) {
          // if (Firebase.success()) {
            delay(1000);
            Firebase_debug("success-remove-data");
            last_resettime = resettime;
            delay(1000);
            Firebase_Send_Distance(new_distance);
          }
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
  //long time_from_reset;
  
  if (server_epoch > 0) {
    last_server_epoch = server_epoch;
    last_server_epoch_elapsed = millis();
  } else {
    server_epoch = last_server_epoch + (millis() - last_server_epoch_elapsed) / 1000;
  }
  
  //time_from_reset = server_epoch - last_resettime; //  resettime;
  //Serial.print("Distance time from reset: "); Serial.println(time_from_reset);

  //if ( time_from_reset > 10000000 || time_from_reset < 0 ) return false; // if something wrong
      
  //String sendstr = "dist:" + String(new_distance) + "&time:" + String(time_from_reset);
  String sendstr = "dist:" + String(new_distance) + "&time:" + String(server_epoch);
  Firebase.pushString(firebaseData, "garagedoor/data/", sendstr);
  //Serial.println("==== Updating Firebase");
  //if (Firebase.failed()) {
    // Firebase_fail("Send_Distance");
  //   return false;
  //}
  //if (Firebase.success()) {
    time_firebase_update = millis();
  //}
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
  
  int wall_pin, reset_wall_pin;
  if (Firebase.getString(firebaseData, "garagedoor/pushButtonWallRemote")) {
    wall_pin = firebaseData.stringData().toInt();
  }
  //Serial.print("garage door pin read: "); Serial.println(wall_pin);
  //digitalWrite(RELAY_OPENER, LOW);

  if ( wall_pin > 1) Firebase.setString(firebaseData, "garagedoor/pushButtonWallRemote", "1");

  if (Firebase.getString(firebaseData, "garagedoor/pushButtonWallRemote")) {
    
    reset_wall_pin = firebaseData.stringData().toInt();
  }
  
  if ( reset_wall_pin == 1) { // make sure it has been reset so it does not open/close continuously
     if (Firebase_Door_Pin_Number(wall_pin)) {  // pin number to open the garage
         digitalWrite(RELAY_OPENER, HIGH);
         delay(500);
         digitalWrite(RELAY_OPENER, LOW);
         Firebase.setString(firebaseData, "garagedoor/pushButtonWallRemote", "1");
        
      }
  }
  return true;
}

// ------------------------------------------------------
void Firebase_debug(String debug_msg) {
  Firebase.setString(firebaseData, "garagedoor/debug/", debug_msg);
}


void(* resetFunc) (void) = 0;//declare reset function at address 0

// ============= End of Firebase ================================
