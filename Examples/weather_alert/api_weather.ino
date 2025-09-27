// ==================== compact Forecast =============

String extract_Compact_Message(String newline) {
    JSONVar myJSON = JSON.parse(newline);
    String Name       = (String)myJSON["name"];
    int Temperature   = (int) myJSON["temperature"];
    int Precipitation = (int) myJSON["probabilityOfPrecipitation"]["value"];
    String Wind       = (String) myJSON["windSpeed"];
    String msg    = Name + ":" + String(Temperature) + "F";
    msg += " Rain:" + String(Precipitation) + "%";
    msg += " Wind:" + Wind;
    return msg;
}

// ==================== extract detailed forecast ======

String detailed_Forecast(String newline) {
   JSONVar myJSON = JSON.parse(newline);
   String Name       = (String)myJSON["name"];
   String Detailed   = (String)myJSON["detailedForecast"];
   return Name + ":" + Detailed;
}

// ===================== get Weather ==================

String get_Weather() {
  WiFiClientSecure client;
  String forecast1, forecast2;
  bool found_severe = false;
  char c;
  int index;
  //HTTPClient https;
  //client.setTimeout(100000);

  Serial.print("Connecting to ");
  Serial.println(github_host);
  
  client.setTrustAnchors(&cert);

  if (!client.connect(github_host, github_port)) {
    Serial.println("Connection failed");
    return "";
  }
  //client.flush();

  String url = "/alerts/active/area/MN";
 
  Serial.print("Requesting URL: ");
  //Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + github_host + "\r\nUser-Agent: Usman Suriono suriono@yahoo.com\r\nConnection: close\r\n\r\n");

  Serial.println("==== Request sent");
  //delay(1000);

  String newline = client.readStringUntil('\n'); // Read first line
  //Serial.print(newline);
  if (newline.startsWith("HTTP/1.1 200 OK")) {
    client.readStringUntil('{');
    
    newline = "{";
    while (newline.indexOf("\"severity\": \"Severe\"") == -1) {  // indexOf() returns -1 if not found
      c = client.read();
      newline += c;
      delay(1);
    }
    if ( newline.indexOf("\"severity\": \"Severe\"") != -1) {   // if severe weather
       newline = "";
       while (newline.indexOf("\"description\": ") == -1) {  // indexOf() returns -1 if not found
          c = client.read();
          newline += c;
          delay(1);
       }
    
       newline = "";
       while (newline.indexOf("\"instruction\"") == -1) {  // indexOf() returns -1 if not found
          c = client.read();
          newline += c;
          delay(1);
       }
       newline = newline.substring(1, newline.length() - 14);
       index = newline.indexOf("\\n");
       while (index != -1) {  // indexOf() returns -1 if not found
          newline = newline.substring(0,index) + " " + newline.substring(index+2, newline.length()-1);
          index = newline.indexOf("\\n");
       }
    }
  }
  return newline; // forecast1 + " ===>>> " + forecast2;
}
