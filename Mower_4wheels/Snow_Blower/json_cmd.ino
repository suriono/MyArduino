void process_JSON() {
  if ( myJSON.hasOwnProperty("cmd")) {
    String cmdstr = (const char*) myJSON["cmd"];
     if ( cmdstr == "ON" ) {
        Blower(HIGH);
     } 
     else if (cmdstr == "OFF") {
        Blower(LOW); 
     }
  }
}
