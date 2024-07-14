

#define PINOUT 13

void setup() {
  //start serial connection
  Serial.begin(57600);
  //configure pin 2 as an input and enable the internal pull-up resistor
   pinMode(12, OUTPUT);
   pinMode(13, OUTPUT);
   pinMode(14, OUTPUT);
   pinMode(16, OUTPUT);
   digitalWrite(12, HIGH);
   digitalWrite(13, HIGH);
   digitalWrite(14, HIGH);
   digitalWrite(16, HIGH);
   //delay(5000);
}

void loop() {
  //read the pushbutton value into a variable
  //int sensorVal = digitalRead(2);
  //print out the value of the pushbutton
 delay(1000); Serial.println("Left LOW"); digitalWrite(12, LOW);
 delay(5000); Serial.println("HIGH");digitalWrite(12, HIGH);
 delay(1000); Serial.println("Right LOW"); digitalWrite(13, LOW);
 delay(5000); Serial.println("HIGH");digitalWrite(13, HIGH);
 delay(1000); Serial.println("reverse LOW"); digitalWrite(14, LOW);
 delay(5000); Serial.println("HIGH");digitalWrite(14, HIGH);
 delay(1000); Serial.println("forward LOW"); digitalWrite(16, LOW);
 delay(5000); Serial.println("HIGH");digitalWrite(16, HIGH);
  }
