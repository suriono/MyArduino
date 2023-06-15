// ================== Mesh Initialization ========================
void Mesh_Init() {
  mesh.setDebugMsgTypes(ERROR | DEBUG);  // set before init() so that you can see error messages

  mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA,2);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  mesh.onNodeDelayReceived(&delayReceivedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}

// ======================= Receive Callback ===========================
void receivedCallback(uint32_t from, String & msg) {
  Serial.printf("Received from %u, msg=%s", from, msg.c_str());
  

  //if ( receivedJSON.hasOwnProperty(String(from))) {
  //   unsigned long elapsed = millis() - uint32_t(receivedJSON[String(from)]["lasttime"]);
     //Serial.printf(" Elapsed: %u", elapsed);
  //} 
  //Serial.println();
  
  //receivedJSON[String(from)]["msg"] = msg;
  //receivedJSON[String(from)]["lasttime"] = millis();
  receivedJSON = JSON.parse(msg);

  //if (JSON.typeof(receivedJSON) != "undefined") {
    //String outstr;
    //double yaw = (double) receivedJSON["Yaw"];
    //int elapse = (int) receivedJSON["IMUelapsed"];
   // outstr = String(yaw) + "," + String(elapse) + "<";
    onFlag = !onFlag;
    //Serial.println(outstr);
    //ProminiSerial.print(outstr);
 // }
  
  //if ( (millis() - uint32_t(generalJSON["lastLED"])) > 1000) {
  //   onFlag = !onFlag;
  //   generalJSON["lastLED"]  = millis();
  //} 
}
