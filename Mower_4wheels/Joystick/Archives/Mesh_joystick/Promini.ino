void Promini_Incoming() {  
  if (ProminiSerial.available()) {
     myJSON = JSON.parse(ProminiSerial.readStringUntil('<'));
    // Serial.println(myJSON);
//     isIncomingGood = ( JSON.typeof(myJSON) != "undefined");

  //   #ifdef DEBUGING
        if (JSON.typeof(myJSON) != "undefined") {
           mesh.sendBroadcast(JSON.stringify(myJSON));
           
//          Serial.print("  Joystick data: ");
//          Serial.println(myJSON);
        }
    // #endif
  }
}
