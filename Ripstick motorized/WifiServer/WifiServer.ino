#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>
#define PWM_PIN          0     // D3
#define BREAK_PIN        5     // D1

JSONVar        myJSON;
WiFiServer     server(5001);
WiFiClient     wificlient;
String         request;
int            Magnitude;
unsigned long  last_no_signal = millis();
boolean        Break, last_Break;

void setup() {
  pinMode(PWM_PIN, OUTPUT);
  pinMode(BREAK_PIN, OUTPUT);
  digitalWrite(BREAK_PIN, LOW);
  Serial.begin(57600);
  analogWrite(PWM_PIN, 0);
  WiFi.softAP("Ripstick", "hello5239612");
  Serial.print("AP IP: "); Serial.println(WiFi.softAPIP());
  server.begin();
  analogWriteRange(1023);
}

// ========================= Loop ==========================
void loop() {
  request   = "";
  Magnitude = 0;
  //Break     = false;
  
  wificlient = server.available();

  if(wificlient) {
    request = wificlient.readStringUntil('\n');
    //request.trim();
    myJSON = JSON.parse(request);
    //wificlient.write("OK");
    //wificlient.flush();
    
    Serial.println(request);

    // Set PWM power
    Magnitude = 0;
    if (myJSON.hasOwnProperty("mag")) {
       Magnitude = int(myJSON["mag"]);
       //Serial.println(Magnitude);
       analogWrite(PWM_PIN, Magnitude);
    }

    // To break
    
    if (myJSON.hasOwnProperty("but")) {
      Break = int(myJSON["but"]) != 1;
      if ( last_Break != Break) {
        if (Break) {
          digitalWrite(BREAK_PIN, HIGH);
        } else {
          digitalWrite(BREAK_PIN, LOW);
        }
        //delay(100);
     }
     last_Break = Break;
      //Serial.println(Break);
    }
    
    
    wificlient.write("OK");
    wificlient.flush();
    last_no_signal = millis();
  } else {
    if ( (millis()-last_no_signal) > 500) {
       analogWrite(PWM_PIN, 0);  // no power when no signal
    }
  }
}
