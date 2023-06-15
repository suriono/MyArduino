String inpstr;

// ==================================
boolean incoming_wifi() {
  char inc;
  //static Strinpstr;
  //static unsigned long lastimecurl;
  boolean isValidCommand = false;  
  
  wificlient = server.available();
  if (wificlient) {
    inpstr = wificlient.readStringUntil('<'); 
    //int count = 0;
    //isValidCommand = false;
    //inpstr = "";
    
    //while (wificlient && count < 50000) {
    //  count++;
    //  while (wificlient.available()) {
    //    inc = wificlient.read();  // read the remaining bytes
     //   inpstr += inc;
     //   if (inc == '}') {
//          
      
      
          
     //     count = 100000;
     //   }
     // }
     // delayMicroseconds(100);
    //}
    
    myJSON = JSON.parse(inpstr);
    isValidCommand = (JSON.typeof(myJSON) != "undefined");
    wificlient.write("OK");
    wificlient.flush();
    //if (isValidCommand) {
    //  wificlient.write("Good");
    //} else {
    //  wificlient.write("Baddddd");
    //}
    
    // #ifdef DEBUG
    //  Serial.println(inpstr);
    // #endif
  }
  return isValidCommand;
}
