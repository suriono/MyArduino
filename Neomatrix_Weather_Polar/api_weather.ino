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

// ==================== extract alert forecast ======

String alert_Forecast(String newline) {
   JSONVar myJSON = JSON.parse(newline);
   String severity  = (String)myJSON["severity"];
   String certainty = (String)myJSON["certainty"];
   String headline  = (String)myJSON["headline"];
   int descr_index = newline.indexOf("\"description");  // "description" index
   //int instr_index = newline.indexOf("\"instruction");  // "instruction" index
   String descript = newline.substring(descr_index+16, descr_index+1000 ); // , instr_index); // newline.length()-descr_index);
   int last_desc_index = descript.lastIndexOf('.');
   descript = descript.substring(0, last_desc_index);
   //String descript  = tmpstr; // (String)myJSON["description"];
   Serial.println(" =======description===");
   Serial.println(descript);
   Serial.println(descr_index); //Serial.println(instr_index);
//   Serial.println(descript);
  // String respons  = (String)myJSON["response"];
   return "Severity:" + severity + ", Certainty:" + certainty + ", Headline:" + headline + ", Description:" + descript + "===";
  // return "Severity:" + severity;
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

  String url = "/gridpoints/MPX/109,66/forecast";
 
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
  return " << == Forecast == >> " + forecast1 + " ===>>> " + forecast2;
}

// ===================== get Alert Weather ==================

String get_Alert_Weather() {
  WiFiClientSecure client;
  String tmpstr, forecast2;

  Serial.print("Connecting to ");
  Serial.println(github_host);
  
  client.setTrustAnchors(&cert);

  if (!client.connect(github_host, github_port)) {
    Serial.println("Connection failed");
    return "";
  }
  //client.flush();

  String url = "/alerts/active?zone=MNC053";
 
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
    newline = client.readStringUntil('{');
    //Serial.println("Header:");
    //Serial.println("==========");
    //Serial.println(newline);
   // Serial.print("Before header:"); Serial.println(newline);

    newline = "{";
    for (byte nn=0; nn<300; nn++) {   // other headers
   
      tmpstr = client.readStringUntil(',');
      //Serial.println(tmpstr);
      
      if ( tmpstr.indexOf("\"sent") > 0) {
          break;
      } else if ( tmpstr.length() < 5) {
          return "<< == Alert == >> None ";
      }
    }
   
    newline += client.readStringUntil('}') + '}' + '}';
    newline.replace("\\n", " ");
    newline.replace("...", "");
    newline.replace("*", "");
   
    //Serial.println(newline);
    
    forecast2 = alert_Forecast(newline);
  }
  return "<< == Alert == >> " + forecast2;
}
