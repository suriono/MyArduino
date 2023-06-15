void process_JSON() {
  String cmdstr;
  
  if ( myJSON.hasOwnProperty("cmd")) {
    cmdstr = (const char*) myJSON["cmd"];
    
    if ( cmdstr == "motorstop" ) {
      motorStop();
    }
  } else {
    if ( myJSON.hasOwnProperty("mag")) {
      int amp = (int) myJSON["mag"];
      if ( myJSON.hasOwnProperty("theta")) {
        double theta = (double) myJSON["theta"];
        if ( myJSON.hasOwnProperty("delay")) {
          int motordelay = (int) myJSON["delay"];
          motorRun(amp, theta, motordelay);
        } else {
          motorRun(amp, theta, 10);
        }
      }
    }
  }

  last_valid_json = millis();
}
