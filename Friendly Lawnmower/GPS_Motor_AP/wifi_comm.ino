String inpstr;
int counter;

// ==================================


boolean incoming_wifi() {  
   boolean isValidCommand = false;  
   counter = 0;
   wificlient = server.available();
   while (wificlient) {
       // IPAddress ip = wificlient.remoteIP();
       incomingByte = wificlient.read();
       //counter = 0;
       //do {
       while (incomingByte < 0) {
          delayMicroseconds(100); 
          incomingByte = wificlient.read();
         // counter++;
       } // while (incomingByte < 0) ;
     
       mySerial.write(incomingByte);
       mySerial.flush();
       isValidCommand = true;
       #ifdef DEBUGGING
          Serial.print(incomingByte,DEC); Serial.print(",");
       #endif 
      wificlient = server.available();
   }
  
  return isValidCommand;

}
