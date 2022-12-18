#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>

JSONVar myJSON;

WiFiServer server(5001);
WiFiClient wificlient;
String request;
char   ch_O, ch_K;
bool is_LED_on;
unsigned long lasttime_led = millis();

#define BREAK_PIN     14    // D5
#define LED_PIN       13    // D7

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(57600);
  WiFi.enableAP(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin("Ripstick","hello5239612");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  pinMode(BREAK_PIN, INPUT_PULLUP);  // D5
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  //static bool is_LED_on = true;
  //static unsigned long lasttime_led = millis();
  //static int nn = 0;
  int analogread;

  if (!wificlient.connect("192.168.4.1", 5001)) {
    Serial.println("Connection to Ripstick server FAILED");
    delay(1000);
    return;
  }

  analogread = analogRead(A0) * 4;
  if (analogread > 1000) {
    analogread = 1000;
  }
  myJSON["mag"] = analogread;
  myJSON["but"] = digitalRead(BREAK_PIN); // D5
  Serial.println(myJSON);
  
  
  request = "";
  wificlient.println(myJSON);
  wificlient.flush();
  request += char(wificlient.read());
  request += char(wificlient.read());
  //Serial.println(request);

//  if (request.equals( "OK")) {
  //   Serial.println("Success " + String(nn));
  //} // else no power to the motor
 
  // nn = nn + 1;
  
  if ( (millis()-lasttime_led) > 1000) {
    if (is_LED_on) {
      is_LED_on = false;
      digitalWrite(LED_PIN, HIGH);
    } else {
      is_LED_on = true;
      digitalWrite(LED_PIN, LOW);
    }
    lasttime_led = millis();
  }
}
