
// ------------------------------------------------------
void Firebase_debug(String debug_msg) {
  Firebase.setString("hometemp/debug/", debug_msg);
}

// ------------------------------------------------

bool Firebase_Send_Temperature(float new_temp) {

  //unsigned long server_epoch = get_Server_Time();

  //if (server_epoch > 0) {
  //  last_server_epoch = server_epoch;
  //  last_server_epoch_elapsed = millis();
  //} else {
  //  server_epoch = last_server_epoch + (millis() - last_server_epoch_elapsed) / 1000;
  //}

  String sendstr = "Temp:" + String(new_temp) + "&time:" + String(get_server_epoch());
  Firebase.pushString("hometemp/data/", sendstr);
  //time_firebase_update = millis();
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

void(* resetFunc) (void) = 0;//declare reset function at address 0

// ============= End of Firebase ================================
