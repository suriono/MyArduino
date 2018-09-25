//#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
//#include <ArduinoJson.h>
//#include <SoftwareSerial.h>

//SoftwareSerial SerialSoft(0, 5); // RX, TX

WiFiServer server(8000);
WiFiClient client;

char* get_SSID();
String get_WiFi_Password();

String Incoming_Data;

void setup() {
  Serial.begin(57600);
  Serial1.begin(38400);  // D4
  //SerialSoft.begin(57600);
  //SerialSoft.setTimeout(10);
  //while(Serial.available()) Serial.read();  // read leftover data

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
  static unsigned long lastimecurl;
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
      //Serial.println(inc);
      if (inc == '{') {
        inpstr = inc;
        lastimecurl = millis();
      } else if (inc == '}') {
        inpstr += inc;
        //Serial.print("robot input: "); Serial.println(inpstr);
        //delay(10);
        isValidCommand = true;
        break;
      } else {
        inpstr += inc;
      }
      //delayMicroseconds(10);
    }
    //Serial.print("count:"); Serial.println(count);

    if (isValidCommand) {
      Serial1.print(inpstr);
      Serial.print("robot input: "); Serial.println(inpstr);

      /*
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& jsonob = jsonBuffer.parseObject(inpstr);

      if (!jsonob.success()) {
        Serial.println("parseObject() failed");
      } else {
        int mag = jsonob["mag"];
        int angle = jsonob["angle"];
        int button = jsonob["button"];
        Serial.print("count:"); Serial.print(count);Serial.print(",");
        Serial.print(mag); Serial.print(",");
        Serial.print(angle); Serial.print(",");
        Serial.println(button);
      }    
      */
    }  
  } else {
    delay(1);
  }
}
