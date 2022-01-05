void WiFi_Setup() {
   WiFi.mode(WIFI_STA);
   // WiFi.begin(ssid, passwd); it's hidden in the lines below
   setCredential();

   while (WiFi.status() != WL_CONNECTED) {
      Serial.print("Attempting to connect to WPA SSID: ");
      Serial.print("Status = ");
      Serial.println(WiFi.status());
      delay(1000);
   }
   Serial.println ("Connected");
   Serial.println(WiFi.localIP());

   server.begin();
   Serial.println("Server started");
   delay(1000);
}

// =========================================================

void WiFi_SendLocation() {
  // send a standard http response header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");  // the connection will be closed after completion of the response
  //client.println("Refresh: 10");  // refresh the page automatically every 5 sec
  client.println();
  //client.println("<!DOCTYPE HTML>");
  //client.println("<html>");

  //unsigned long sendtime = millis();

  client.print("LT ");
  client.print(Lattitude,8);
  client.print(",LN ");
  client.print(Longitude,8);
  client.print(",AC ");
  client.print(Accuracy);
  client.print(",RP ");
  client.print(Response);
}
