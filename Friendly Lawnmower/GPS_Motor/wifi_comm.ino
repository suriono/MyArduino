String inpstr;

// ==================================


boolean incoming_wifi() {
  
  boolean isValidCommand = false;  

  wificlient = server.available();

// Serial.println("==========================");
 static byte counter = 0;
 if (wificlient) {
    incomingByte = wificlient.read();
  //  inpstr = wificlient.readStringUntil('<');
    if (incomingByte > -1) { 
        counter = (counter + 1) % 40; 
        Serial.print(incomingByte,DEC); Serial.print("("); Serial.print(counter); Serial.print("),");
        if (counter < 1) {
           Serial.println("--------");
        }
        isValidCommand = true;
    } 
   // delay(1);
    //wificlient = server.available();


   // if (inpstr == "OK") {
      #ifdef DEBUGGING
         // Serial.println();
      //    Serial.print("================="); Serial.print(inpstr); Serial.print(" , elapsed: "); Serial.println(millis()-last_incoming_wifi);
      #endif 
     
   }
   //Serial.println();
   last_incoming_wifi = millis();
  //}
  return isValidCommand;

}
