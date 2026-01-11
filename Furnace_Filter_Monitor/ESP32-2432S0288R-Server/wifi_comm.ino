
String inpstr;

boolean incoming_wifi() {
  
  boolean isValidCommand = false;  

  wificlient = server.available();

  if (wificlient) {
      inpstr = wificlient.readStringUntil('<'); 
      Serial.print("Incoming:"); Serial.println(inpstr);
      isValidCommand = JSON_Process(inpstr);
      //myJSON = JSON.parse(inpstr);
      //isValidCommand = (JSON.typeof(myJSON) != "undefined");
  }

  return isValidCommand;
}
