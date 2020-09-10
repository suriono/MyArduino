void Wifi_init() {
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
  delay(200);
  Serial.println("Server started");
}

// ==============================

void incoming_client() {
  Serial.println("Incoming client ...");

  char readchar;
  String readstr, datastr;
  int count = 0;
  bool isStartData = false;
  bool isEndData   = false;

  while (client.connected() && count < 50000) {
     count++;
     if(client.available()) {
        readchar = client.read();
        readstr += readchar;
        if (readstr.indexOf("<endsign>") > 100) {
           isEndData = true;
           break;
        }
        if (isStartData) {
           datastr += readchar;
        } else if(readstr.indexOf("<sign>") > 100) {
           isStartData = true;
        }
     } else {
        delayMicroseconds(10);  // if it goes too fast
     }
   }
   if (isStartData && isEndData) {
      String formatstr = datastr.substring(0, datastr.indexOf("<endsign"));
      Serial.println(formatstr);
      Serial1.print(formatstr);
      //Serial1.flush();
   }
}
