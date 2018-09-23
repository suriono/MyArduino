/* NodeMCU pins mapping to Arduino
D0   = 16;    D1   = 5;     D2   = 4;
D3   = 0;     D4   = 2/TX1;     D5   = 14;    D6   = 12;
D7   = 13;    D8   = 15;    D9   = 3;     D10  = 1;
*/
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
//#include <ArduinoJson.h>

WiFiClient client;

SoftwareSerial mySerial(4, 5); // RX, TX, 4=D2 for NodeMCU

#define HOST_IP     "192.168.4.100"
#define HOST_PORT   8000

void setup() {
  Serial.begin(57600);
  Serial.println("Starting ...");
  // software serial to read from joystick pro mini
  mySerial.begin(9600);
  mySerial.setTimeout(100);
  
  // We start by connecting to a WiFi network
  WiFi.setOutputPower(20.0); 
  WiFi.enableAP(false); // required
  WiFi.mode(WIFI_STA);
  //WiFi.begin("philip", "fern5077266123");
  WiFi.begin("Robot_AP", "hellorobot");
  
  while(WiFi.status() != WL_CONNECTED) {
    Serial.println("joystick nodemcu waiting to connect");
    delay(500);
  }
  client.setTimeout(20);
  Serial.println(""); Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  delay(1000);
}

//unsigned long lastimecurl = millis();
void loop() { // run over and over
  char inc;
  static String inpstr;
  static unsigned long lastimecurl;
  static boolean isValidCommand = false;

 if ( (millis() - lastimecurl) > 2000) { // clear input string if too long
   inpstr = "";
 }
  
  if (mySerial.available()) {
    while(mySerial.available()) {
      inc = mySerial.read();  // read the remaining bytes
      //Serial.println(inc);
      if (inc == '{') {
        inpstr = inc;
        lastimecurl = millis();
      } else if (inc == '}') {
        inpstr += inc;
        //Serial.print("joystick: "); Serial.println(inpstr);
        /*
        StaticJsonBuffer<200> jsonBuffer;
        JsonObject& jsonob = jsonBuffer.parseObject(inpstr);
        int mag = jsonob["mag"];
        int angle = jsonob["angle"];
        int button = jsonob["button"];
        Serial.print(mag); Serial.print(",");
        Serial.print(angle); Serial.print(",");
        Serial.println(button);
        */
        isValidCommand = true;
      } else {
        inpstr += inc;
      }
    }
    //Serial.print("incoming:");Serial.print(inpstr);Serial.println("==");
    if (isValidCommand) {
      isValidCommand = false;

      if (!client.connect(HOST_IP, HOST_PORT)) {
      //if (client.connect(HOST_IP, HOST_PORT)) {
      //if (!client.connect("192.168.4.1", 8000)) {
        Serial.println("connection to server failed");
        //Serial.println("wait 5 sec...");
        //delay(5000);
      } else if (client.connected()) {
/*
        inputstr.replace("joystick=","");
        int inputint = inputstr.toInt();
        int mag = inputint & 127;
        int angle = ((inputint >> 7) & 127) * 3;
        boolean button_press = inputint >> 14;
        Serial.print("Mag="); Serial.print(mag); Serial.print(",");
        Serial.print("Angle="); Serial.print(angle); Serial.print(",");
        Serial.print("Button press="); Serial.print(button_press);
        int anglespeed = (button_press << 14) + ((angle / 3) << 7) + mag;
        
        // This will send the request to the robot
        Serial.print(", anglespeed="); Serial.println(anglespeed);
        client.print("anglespeed=" + String(anglespeed));
        */
        Serial.print("Sending to server");
        Serial.println(inpstr);
        client.print(inpstr);
        client.flush();
        //client.stop();
        while(client.available()) { // read any incoming serial buffer
            client.read();
        }
        inpstr = "";
      } else {
        delay(1);
        Serial.println(" ===== not connected"); 
      }
    }
  } else {
    delay(1);
  }
}
