void Blower(boolean OnOff) {
  digitalWrite(PIN_SNOWBLOWER, OnOff);
  Serial.print("The blower is: "); Serial.println(OnOff);
}
