void setup() {
  Serial.begin(19200);
  Serial.swap(); // (Rx2,D7) and 15(Tx2,D8)
  delay(1000); // Wait for a second
  Serial1.begin(19200);
}

unsigned long lasttime;
void loop() {
  if (Serial.available()) {
    Serial1.write(Serial.read());
  } else if ( (millis()-lasttime) > 1000 ) {
    lasttime = millis();
    Serial.println("hello");
  }

}
