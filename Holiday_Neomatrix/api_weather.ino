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

   // String url = "api.tomtom.com/routing/1/calculateRoute/44.742127,-93.194999:44.745049,-93.194981/json?key=5oS0hZX2lCeGfmjVWxaAbvRvGKSSlSkB&travelMode=car&sectionType=traffic&sectionType=speedLimit&report=effectiveSettings&vehicleEngineType=combustion";
  String url = "/gridpoints/MPX/111,61/forecast";
 
  Serial.print("Requesting URL: ");
  //Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + github_host + "\r\nUser-Agent: Usman Suriono suriono@yahoo.com\r\nConnection: close\r\n\r\n");

  Serial.println("==== Request sent");
  delay(1000);

  String newline = client.readStringUntil('\n'); // Read first line
  //Serial.print(newline);
  if (newline.startsWith("HTTP/1.1 200 OK")) {
    // Serial.println(newline);
    // Serial.println("HTTP 200 success, header: ");
    client.readStringUntil('{');
    //Serial.println("Header:");
    //Serial.println("==========");
    //Serial.println(newline);

    newline = "{";
    for (byte nn=0; nn<3; nn++) {   // other headers
      client.readStringUntil('}') + "}";
    }
    
    // ====== Today =============================
    client.readStringUntil('{');
    newline = "{" + client.readStringUntil('}') + "}";
    newline += client.readStringUntil('}') + "}}";
    Serial.println("Out bound message: ================");
    forecast1 = extract_Compact_Message(newline);

    // ===== Next forecast ======================
    newline = client.readStringUntil('}') + "}";
    newline.remove(0,1);
    newline += client.readStringUntil('}') + "}}";
    forecast2 = detailed_Forecast(newline);
    //Serial.println(extract_Message(newline));
   // Serial.println("Content: ========================="); Serial.println(newline);
  }
  return forecast1 + " ===>>> " + forecast2;
}
