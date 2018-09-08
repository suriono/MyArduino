// ============= Take ultra sonic distance in cm =================

int Get_Sonic_Distance() {
  unsigned long curtime = millis();
  while ( (millis()-curtime) < 20000) { // up to 20 seconds
    if (SerialSonar.available()) {
      int dist = SerialSonar.parseInt();
      if (dist <400 && dist > 0) return dist;
    }
    delay(500);
  }
  return 400;
}
