String inpstr, outstr;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                           // ==================================
boolean incoming_wifi() {
  
  boolean isValidCommand = false;  
  
  wificlient = server.available();
  if (wificlient) {
    inpstr = wificlient.readStringUntil('<'); 
    myJSON = JSON.parse(inpstr);
    isValidCommand = (JSON.typeof(myJSON) != "undefined");
  }
  return isValidCommand;
}

// ==================================

void send_coordinates() {
  if (wificlient) {
    outstr = "{\"lat\":" + String(d_lat,9) + ",\"lon\":" + String(d_lon,9) + ",\"prec\":" + String(accuracy) + "}";
    Serial.print("Sending: "); Serial.println(outstr);
    wificlient.print(outstr);
    wificlient.flush();
  }
}

// ==================================

void send_rtk() {
  if (wificlient) {
    byte RTK = myGNSS.getCarrierSolutionType();
    outstr = "{\"rtk\":" + String(RTK) + "}";
    Serial.print("Sending: "); Serial.println(outstr);
    wificlient.print(outstr);
    wificlient.flush();
  }
}
