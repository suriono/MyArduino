void Nextion_Loop() {
  static unsigned long lasttime;
  myNex.NextionListen(); // WARNING: This function must be called repeatedly to response touch events
                         // from Nextion touch panel. Actually, you should place it in your loop function.
  if ( (millis()-lasttime)> 1000) {
    lasttime = millis();
    Max_Speed = myNex.readNumber("h0.val");
    #ifdef DEBUG
      Serial.print("Max Speed:"); Serial.println(maxSpeed);
    #endif
    myNex.writeNum("n0.val", Max_Speed);
    //myNex.writeNum("n0.val", 27);
  }
}

// ========================== Max Speed buttons ==================================================
void trigger0(){
  myNex.writeNum("h0.val", 20);
}

void trigger1(){
  myNex.writeNum("h0.val", 50);                                            
}

void trigger2(){
  myNex.writeNum("h0.val", 100);
}
