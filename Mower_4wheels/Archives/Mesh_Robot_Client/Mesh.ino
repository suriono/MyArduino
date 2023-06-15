// ================== Mesh Initialization ========================
void Mesh_Init() {
  mesh.setDebugMsgTypes(ERROR | DEBUG);  // set before init() so that you can see error messages

  mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA,2);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  mesh.onNodeDelayReceived(&delayReceivedCallback);
  userScheduler.addTask( taskPeriodicRun );
  taskPeriodicRun.enable();
}

// ======================= Receive Callback ===========================
void receivedCallback(uint32_t from, String & msg) {
//  Serial.printf("=============: Received from %u, msg=%s", from, msg.c_str());
  
  myJSON = JSON.parse(msg);
  if (JSON.typeof(myJSON) != "undefined") {
    process_JSON();
    LEDonoff = !LEDonoff;
//    String IMU_str ="{\"Yaw\":" + String(YAW) + ",\"IMUelapsed\":" + String(elapsed) + "}";
  //  String IMU_str ="{\"Yaw\":" + String(YAW) + ",\"IMUelapsed\":" + String(elapsed) + "}";
    //mesh.sendBroadcast("{\"Yaw\":" + String(YAW) + "}" ); // sending OK from robot 
    mesh.sendBroadcast("\{\"robotMsg\":\"OK\"\}"); // sending OK from robot  
  } else {
    #ifdef DEBUGGING
      Serial.print("Undefined command received:"); 
      Serial.println(msg);
    #endif
  }

//  if ( timeJSON.hasOwnProperty(String(from))) {
//      unsigned long elapsed = millis() - uint32_t(timeJSON[String(from)]["lasttime"]);
//      Serial.printf(" Elapsed: %u", elapsed);
//  }
//  timeJSON[String(from)]["msg"] = msg;
//  timeJSON[String(from)]["lasttime"] = millis();
  
  //onFlag = !onFlag;
}
