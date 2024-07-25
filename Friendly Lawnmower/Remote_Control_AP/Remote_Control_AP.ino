#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>
#define DEBUGGING

#define FWD_PIN         5    // D1 Forward direction
#define RVS_PIN         4    // D2 Reverse direction
#define RIG_PIN         3    // RX right direction
#define LEF_PIN         1    // TX left direction
#define MOW_PIN         0    // D3 to mow


//#define HOST_IP        "192.168.0.192"
#define HOST_IP        "192.168.4.1"
#define HOST_PORT      8000
#define WIFI_INTERVAL  250

JSONVar myJSON;
WiFiClient wificlient;
unsigned long lasttime_ok;
unsigned long lasttime_led = millis();

void setup() {
  Serial.begin(57600); 
  WiFi.enableAP(false);
  WiFi.mode(WIFI_STA);
  wifi_begin();
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  wificlient.setTimeout(200); 
  pinMode(FWD_PIN, INPUT_PULLUP);
  pinMode(RVS_PIN, INPUT_PULLUP);
  pinMode(RIG_PIN, INPUT_PULLUP);
  pinMode(LEF_PIN, INPUT_PULLUP);
  pinMode(MOW_PIN, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);

  //JSON_send_key_value("Fwd", 1);
  delay(1000);
  while (!wificlient.connect(HOST_IP, HOST_PORT)) {
      #ifdef DEBUGGING
         Serial.println("\t,  Connection to robot failed !!!!");
      #endif
      delay(1000);
   }
   delay(1000);
  //JSON_send_key_value("Fwd", 0);
  Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());
  
}

// ============================= Loop ==================================

void loop() {
  static bool is_OK;
  static int ledState = LOW;      
  boolean fwd_onoff; // = digitalRead(FWD_PIN);
  boolean rvs_onoff;// = digitalRead(RVS_PIN);
  boolean right_onoff, left_onoff, mow_onoff;

  fwd_onoff   = digitalRead(FWD_PIN);
  rvs_onoff   = digitalRead(RVS_PIN);
  right_onoff = digitalRead(RIG_PIN);
  left_onoff  = digitalRead(LEF_PIN);
  mow_onoff   = digitalRead(MOW_PIN);

  //lasttime_ok = millis() ; 
  
  //delay(400);
  is_OK = false;

  if (!fwd_onoff) {
    Serial.println("Foward");
    is_OK = JSON_send_key_value("Run", 1);
   // if (JSON_send_key_value("Run", 1)) {  // forward 
   //    lasttime_ok = millis();
       
  //  }
  } else if (!rvs_onoff) {
    Serial.println("  Reverse");
    is_OK = JSON_send_key_value("Run", 2);
   // if (JSON_send_key_value("Run", 2)) lasttime_ok = millis() ; // reverse
  } else if (!right_onoff) {
    Serial.println("  Right");
    is_OK = JSON_send_key_value("Run", 3);
  } else if (!left_onoff) {
    Serial.println("  Left");
    is_OK = JSON_send_key_value("Run", 4);
  } else if (!mow_onoff) {
    Serial.println("  Mow");
    is_OK = JSON_send_key_value("Run", 5);
  } else {
    Serial.println("    Stop");
    is_OK = JSON_send_key_value("Run", 0);
   // if (JSON_send_key_value("Run", 0)) lasttime_ok = millis() ; // else stop
  }
  if (is_OK) {
    if ( (millis() - lasttime_led) > 1000) {
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
      //Serial.print(ledState); Serial.println (" ---- OKKKKK");
      digitalWrite(LED_BUILTIN, ledState);
      lasttime_led = millis();
    }
    //Serial.println("   Succesful");
    delay(200);
  } else {
    delay(400);
  }
  // put your main code here, to run repeatedly:

}
