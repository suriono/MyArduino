#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <SabertoothSimplified.h>


WiFiServer server(8000);
WiFiClient client;
SabertoothSimplified ST(Serial1); // Use SWSerial as the serial port.
#define  MOWER_PIN  4             // D2 = 4

char* get_SSID();
String get_WiFi_Password();
String Incoming_Data;
StaticJsonDocument<200> jsonparse;

unsigned long last_mower_toggle, last_motor_run;
boolean Mower_Toggle = false;

#define DEBUG

void setup() {
  Serial.begin(57600);
  Serial1.begin(38400);                 // D4
  motorStop();
 // pinMode(MOWER_PIN, OUTPUT);     // D2 = 4
  pinMode(MOWER_PIN, INPUT_PULLUP);
  //SerialSoft.begin(57600);
  //SerialSoft.setTimeout(10);
  //while(Serial.available()) Serial.read();  // read leftover data
  //digitalWrite(MOWER_PIN, HIGH);

  //ESP.eraseConfig(); delay(1000); // needed if it keeps old setup
  Serial.print("Auto Connect: "); Serial.println(WiFi.getAutoConnect());
  WiFi.enableAP(false);
  WiFi.mode(WIFI_STA);
  IPAddress ip(192, 168, 4, 100); 
  IPAddress gateway(192, 168, 4, 1); // set gateway to match your network
  IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your
  WiFi.config(ip, gateway, subnet);
  WiFi.setOutputPower(5.0);  // 0.5 to 20 Output WiFi power, it's near AP
  WiFi.begin("Robot_AP", "hellorobot");
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

// ======================= Loop ==============================

void loop() {
  char inc;
  static String inpstr;
  static unsigned long lastimecurl, last_toggle;
  static boolean isValidCommand = false;

  if ( (millis() - lastimecurl) > 2000) { // clear input string if too long
    inpstr = "";
  }
  
  client = server.available();
  if (client) {
    int count = 0;
    isValidCommand = false;
    while (client.connected() && count < 50000) {
      count++;
      inc = client.read();  // read the remaining bytes
      if (inc == '{') {
        inpstr = inc;
        lastimecurl = millis();
      } else if (inc == '}') {
        inpstr += inc;
        isValidCommand = true;
        break;
      } else {
        inpstr += inc;
      }
    }
    //Serial.println(count);
    client.stop();
    
    if (isValidCommand) {
      deserializeJson(jsonparse, inpstr);
      int mag  = jsonparse["mag"];
      int angle = jsonparse["angle"];
      int enable_mower = jsonparse["button"];

      #ifdef DEBUG
        //Serial.print("robot input: "); Serial.println(inpstr);
        //Serial.print("mag: ");Serial.print(mag);
        //Serial.print(" , angle: ");Serial.println(angle);
      #endif
      unsigned long tmp = millis()-last_motor_run;
      int new_delay = constrain(150 - tmp, 5, 100);
      motorRun(mag, angle, new_delay);

      if (enable_mower==0 && (millis() - last_toggle) > 500) {
        Mower_Toggle = !Mower_Toggle;
        last_toggle = millis();
      
        if (Mower_Toggle) {
          pinMode(MOWER_PIN, OUTPUT);           // D2 = 4
          digitalWrite(MOWER_PIN, LOW);
        } else {
          pinMode(MOWER_PIN, INPUT_PULLUP);     // D2 = 4
        }
      }
    }  
  } else if ( (millis()-last_motor_run)>200 ) {
    motorStop();
  //} else {
    //delay(1);
  }
}
