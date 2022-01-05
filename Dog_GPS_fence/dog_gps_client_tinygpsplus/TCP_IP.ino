

// ============= Write to Client =================
void write_client() {
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
  client.print(last_lat,8);
  client.print(",LN ");
  client.print(last_lon,8);
  client.print(",TM ");
  client.print(last_time_gps);

  //client.println("<br/></html>");
}

// =============== Initial Setup =========================

void WiFi_Setup() {
  //ESP.reset(); delay(5000);   // if necessary
  //ESP.eraseConfig(); delay(1000); // needed if it keeps old setup
  Serial.print("Auto Connect: "); Serial.println(WiFi.getAutoConnect());
 
  WiFi.setOutputPower(20.0);  // 0.5 to 20 Output WiFi power, it's near AP
  WiFi.enableAP(false);
  WiFi.mode(WIFI_STA);
  //IPAddress ip(192, 168, 4, 100); 
  //IPAddress gateway(192, 168, 0, 1); // set gateway to match your network
  //IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your
  //WiFi.config(ip, gateway, subnet);
  WiFi.begin("philip","fern5077266123");
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
    
  // Start the server
  server.begin();
  Serial.println("Server started");

  // buzz once to indicate the WiFi is ready
  last_buzz = Buzz_Delay(100, 200, 1000); // freq, amplitude (0-1023), delay (ms)
  delay(1000);
}
