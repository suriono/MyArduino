String inpstr, Outstr;

// ==================================
boolean incoming_wifi() {
  
  boolean isValidCommand = false;  
  
  wificlient = server.available();
  if (wificlient) {
    inpstr = wificlient.readStringUntil('<'); 
    myJSON = JSON.parse(inpstr);
    isValidCommand = (JSON.typeof(myJSON) != "undefined");
    //Serial.print(myJSON);
  }
  return isValidCommand;
}

// =================== Send Yaw =========
void Yaw_send() {
   if (wificlient) {
      Outstr  = "{\"yaw\":"      + String(Yaw);
      Outstr += "}<";
      wificlient.print(Outstr);
      wificlient.flush();
   }
}

void Yaw_status(int yawstatus) {
    if (wificlient) {
      Outstr  = "{\"yawstatus\":" + String(yawstatus);
      Outstr += "}<";
      wificlient.print(Outstr);
      wificlient.flush();
   }
}
