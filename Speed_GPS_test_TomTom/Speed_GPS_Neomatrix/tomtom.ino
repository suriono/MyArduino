

int get_Speed_Limit(char coord_char[]) {
  int mph = 0;
  // Set time via NTP, as required for x.509 validation
//  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

//  Serial.print("Waiting for NTP time sync: ");
//  time_t now = time(nullptr);
//  while (now < 8 * 3600 * 2) {
//    delay(500);
//    Serial.print(".");
//    now = time(nullptr);
//  }
//  Serial.println("");
//  struct tm timeinfo;
//  gmtime_r(&now, &timeinfo);
//  Serial.print("Current time: ");
//  Serial.print(asctime(&timeinfo));

  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  HTTPClient https;
  client.setTimeout(100);

  Serial.print("Connecting to ");
  Serial.println(github_host);

  //Serial.printf("Using certificate: %s\n", cert_DigiCert_Global_Root_CA);
  client.setTrustAnchors(&cert);

  if (!client.connect(github_host, github_port)) {
    Serial.println("Connection failed");
    return 0;
  }

 // String url = "api.tomtom.com/routing/1/calculateRoute/44.742127,-93.194999:44.745049,-93.194981/json?key=5oS0hZX2lCeGfmjVWxaAbvRvGKSSlSkB&travelMode=car&sectionType=traffic&sectionType=speedLimit&report=effectiveSettings&vehicleEngineType=combustion";
 //String url = "/routing/1/calculateRoute/44.742127,-93.194999:44.745049,-93.194981/json?key=5oS0hZX2lCeGfmjVWxaAbvRvGKSSlSkB&travelMode=car&sectionType=traffic&sectionType=speedLimit&report=effectiveSettings&vehicleEngineType=combustion";
 String url = "/routing/1/calculateRoute/" + String(coord_char) + "/json?key=5oS0hZX2lCeGfmjVWxaAbvRvGKSSlSkB&travelMode=car&sectionType=traffic&sectionType=speedLimit&report=effectiveSettings&vehicleEngineType=combustion";
 
 // Serial.print("Requesting URL: ");
 // Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + github_host + "\r\nUser-Agent: ESP8266SpeedLimit\r\nConnection: close\r\n\r\n");
 //client.print(String("GET ") + "\r\n" + "Connection: close\r\n\r\n");
 
 // Serial.println("==== Request sent");
  delay(500);
 // while (client.available()) {
   
 //   String line = client.readStringUntil('\n');
 //   Serial.println(line);
 //   if (line == "\r") {
 //     Serial.println("Headers received");
 //     break;
 //   }
 // }
 // Serial.println("======= End of header =============");
  
  String newline = client.readStringUntil('\n'); // Read first line
  if (newline.startsWith("HTTP/1.1 200 OK")) {
    // Serial.println(newline);
    // Serial.println("HTTP 200 success, header: ");
    newline = client.readStringUntil('{');
    //Serial.println("Header:");
    //Serial.println("==========");
    //Serial.println(newline);

    //newline = "{";
    
    //newline += client.readStringUntil('\r');
    //newline += client.readString();
    //Serial.println("Content: "); Serial.println(newline);

    JSONVar myJSON = JSON.parse("{" + client.readString());

   // if (JSON.typeof(myJSON) == "undefined") {
  //    Serial.print("=== JSON parsing failed: "); Serial.println(newline);
 //  } else {
     // Serial.print("===== JSON=== PASSED: "); Serial.println(newline);
      
     // Serial.println(myJSON["routes"][0]["sections"][0]["maxSpeedLimitInKmh"]);

      if (JSON.typeof(myJSON["routes"][0]["sections"][0]["maxSpeedLimitInKmh"]) != "undefined") {
        int speedLimit = (int) myJSON["routes"][0]["sections"][0]["maxSpeedLimitInKmh"];
        mph = int(round(speedLimit * 0.6214));
     //   Serial.print("Speed Limit kmh : "); Serial.println(speedLimit);
      //  Serial.print("Speed Limit mph : "); Serial.println(mph);
        //return mph;
      }
   // }
  } else {
    Serial.println("====== HTTPs failed");
  }
 
  return mph;
}
