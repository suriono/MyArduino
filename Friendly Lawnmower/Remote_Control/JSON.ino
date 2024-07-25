String inpstr;

bool JSON_send_key_value(String key, int value) {
//   if (!wificlient.connect(HOST_IP, HOST_PORT)) {
//      #ifdef DEBUG
//         Serial.println("\t,  Connection to robot failed !!!!");
//      #endif
//      delay(1);
//      return;
//   }
   if ( wificlient.connect(HOST_IP, HOST_PORT)) {
      wificlient.print("{\"" + key + "\":" + String(value) + "}<");
      wificlient.flush();
     // delay(10);
      inpstr = wificlient.readStringUntil('<'); 
      #ifdef DEBUGGING
         Serial.println(inpstr);
      #endif
      if (inpstr == "OK") {
        return true;
      }
   //} else {
   //delay(100);
    //  wificlient.flush();
   }
   return false;

  // while(wificlient.available()) {
   //   char c = wificlient.read();
   //   Serial.print(c);
   //}
}
