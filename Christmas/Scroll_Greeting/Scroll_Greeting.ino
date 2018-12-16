#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define PIN 5

WiFiServer server(80);
WiFiClient client;

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, PIN,
  NEO_MATRIX_BOTTOM    + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

String Incoming_Data;

void setup() {
  Serial.begin(57600);

  Incoming_Data = "Merry Christmas";

  matrix.begin();
  matrix.setTextWrap(true);
  matrix.setBrightness(8);
  matrix.setTextColor(matrix.Color(255,0,0));
  matrix.fillScreen(0);
  matrix.setCursor(0, 0);
  matrix.print(F("Hello"));
  matrix.show();
 
  //ESP.eraseConfig(); delay(1000); // needed if it keeps old setup
  Serial.print("Auto Connect: "); Serial.println(WiFi.getAutoConnect());

  WiFi.setOutputPower(20.0);  // 0.5 to 20 Output WiFi power, it's near AP
  WiFi.enableAP(false);
  WiFi.mode(WIFI_STA);
  IPAddress ip(192, 168, 254, 200); 
  IPAddress gateway(192, 168, 254, 254); // set gateway to match your network
  IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your
  WiFi.config(ip, gateway, subnet);
  Wifi_Begin();  // see the password file not in the github
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
  delay(2000);
  Serial.println("Server started");
}

void loop() {
  static unsigned long last_scroll;
  client = server.available();
  if (client) {
    incoming_client();
  } else if ( (millis()-last_scroll) > 100) {
    last_scroll = millis();
    delay(1);
    scroll_text();
  }
}
