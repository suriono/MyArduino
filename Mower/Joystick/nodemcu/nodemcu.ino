/* NodeMCU pins mapping to Arduino
D0   = 16;    D1   = 5;     D2   = 4;
D3   = 0;     D4   = 2/TX1;     D5   = 14;    D6   = 12;
D7   = 13;    D8   = 15;    D9   = 3;     D10  = 1;
*/
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

WiFiClient client;

SoftwareSerial mySerial(4, 5); // RX, TX, 4=D2 for NodeMCU

#define SSID_ROBOT  "Robot_AP"
#define PASSWORD    "xxxx"
#define HOST_IP     "192.168.4.1"
#define HOST_PORT   8000

void setup() {
  Serial.begin(57600);
  Serial.println("Starting ...");
  // software serial to read from joystick pro mini
  mySerial.begin(9600);
  mySerial.setTimeout(100);

  // We start by connecting to a WiFi network
  // WiFi.begin("siantar", "aza5077266123");
  WiFi.enableAP(false); // required
  WiFi.begin(SSID_ROBOT, PASSWORD);
  // WiFi.begin(SSID_ROBOT, PASSWORD);
  
  while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
  }

  client.setTimeout(20);

  Serial.println(""); Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  
  delay(1000);
}

unsigned long lastime = millis();
void loop() { // run over and over
  
  if (mySerial.available()) {
    String inputstr = mySerial.readStringUntil('<');
    while(mySerial.available()) {
      mySerial.read();  // read the remaining bytes
    }
    if (inputstr.startsWith("joystick=")) {

      if (!client.connect(HOST_IP, HOST_PORT)) {
      //if (!client.connect("192.168.4.1", 8000)) {
        Serial.println("connection failed");
        Serial.println("wait 5 sec...");
        delay(5000);
      } else if (client.connected()) {

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
        client.flush();
        //client.stop();
         while(client.available()) { // read any incoming serial buffer
            client.read();
        }
      } else {
        delay(1);
        Serial.println(" ===== not connected"); 
      }
    }
  } else {
    delay(10);
  }
}

