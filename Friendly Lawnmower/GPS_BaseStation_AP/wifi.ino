//String inpstr;
//unsigned long last_incoming_wifi = millis();
//
//// ==================================
//
//
//boolean incoming_wifi() {
//  
//  boolean isValidCommand = false;  
//
//  wificlient = server.available();
//
//  if (wificlient) {
//    inpstr = wificlient.readStringUntil('<'); 
//    
//
//    if (inpstr == "OK") {
//      #ifdef DEBUGGING
//          Serial.println();
//          Serial.print("================="); Serial.print(inpstr); Serial.print(" , elapsed: "); Serial.println(millis()-last_incoming_wifi);
//      #endif  
//    }
//    last_incoming_wifi = millis();
//  }
//  return true;
//
//}
