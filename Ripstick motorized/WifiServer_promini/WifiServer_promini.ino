#include <ESP8266WiFi.h>

//JSONVar        myJSON;
WiFiServer     server(5001);
WiFiClient     wificlient;
String         request;

void setup() {
  Serial.begin(57600);
  delay(2000);
  WiFi.softAP("Ripstick", "hello5239612");
//  Serial.print("AP IP: "); Serial.println(WiFi.softAPIP());
  server.begin();
}

// ========================= Loop ==========================
void loop() {
  request   = "";
  wificlient = server.available();

  if(wificlient) {
    request = wificlient.readStringUntil('\n');
    Serial.println(request);
    wificlient.write("OK");
    wificlient.flush();
  }
}
