#include <Arduino_JSON.h>

#define PWM_PIN          3     
#define BREAK_PIN        5   

JSONVar        myJSON;
String         request;
int            Magnitude;
unsigned long  last_no_signal = millis();
boolean        Break, last_Break;

void setup() {
  pinMode(PWM_PIN, OUTPUT);
  pinMode(BREAK_PIN, OUTPUT);
  Serial.begin(57600);
  digitalWrite(BREAK_PIN, LOW);
  analogWrite(PWM_PIN, 0);
}

void loop() {
  request   = "";
  Magnitude = 0;
    
  if(Serial.available()) {
    request = Serial.readStringUntil('\n');
    myJSON = JSON.parse(request);
    
    // Set PWM power
    Magnitude = 0;
    if (myJSON.hasOwnProperty("mag")) {
       Magnitude = map(int(myJSON["mag"]),0,1023,0,255);
       //Serial.println(Magnitude);
       analogWrite(PWM_PIN, Magnitude);
       //Serial.println(Magnitude);
    }

    // To break
    if (myJSON.hasOwnProperty("but")) {
      Break = int(myJSON["but"]) != 1;
      //Serial.println(Break);
      if ( last_Break != Break) {
        if (Break) {
          digitalWrite(BREAK_PIN, HIGH);
        } else {
          digitalWrite(BREAK_PIN, LOW);
        }
        //delay(100);
     }
     last_Break = Break;
     last_no_signal = millis();
    }
  } else {
    if ( (millis()-last_no_signal) > 500) {
       analogWrite(PWM_PIN, 0);  // no power when no signal
       last_no_signal = millis();
    }
  }
}
