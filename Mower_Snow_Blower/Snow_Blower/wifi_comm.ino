String inpstr, Outstr;

// ==================================
boolean incoming_wifi() {
  
  boolean isValidCommand = false;  
  
  wificlient = server.available();
  if (wificlient) {
    inpstr = wificlient.readStringUntil('<'); 
    myJSON = JSON.parse(inpstr);
    isValidCommand = (JSON.typeof(myJSON) != "undefined");
    Serial.print(myJSON);
  }
  return isValidCommand;
}
