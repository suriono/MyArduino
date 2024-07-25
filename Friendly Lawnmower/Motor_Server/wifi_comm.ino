String inpstr, Outstr;
char chr;
bool is_LED_on;
unsigned long lasttime_led;

// ==================================
boolean incoming_wifi() {
  
  boolean isValidCommand = false;  

  wificlient = server.available();
  
  if (wificlient) {
  //   chr = wificlient.read();
 //    Serial.println(chr);
//     if (chr == '{') {
//
//    if (chr == '<') {
//       myJSON = JSON.parse(inpstr);
//       process_JSON();
//       inpstr = "";
//       return true;
//    } else {
//       inpstr += chr;
//    }
//    wificlient.print(inpstr);
//  }
    
      inpstr = wificlient.readStringUntil('<'); 
      myJSON = JSON.parse(inpstr);
      isValidCommand = (JSON.typeof(myJSON) != "undefined");
     // Serial.println(inpstr);
     
    // if ( (millis()-lasttime_led) > 1000) {
   //     if (is_LED_on) {
   //        is_LED_on = false;
      //     digitalWrite(LED_BUILTIN, HIGH);
   //     } else {
    //       is_LED_on = true;
        //   digitalWrite(LED_PIN , LOW);
     //   }
     //   lasttime_led = millis();
    //    Serial.println(is_LED_on);
    //  }
  }
  return isValidCommand;
}
