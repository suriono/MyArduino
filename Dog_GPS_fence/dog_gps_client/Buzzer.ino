#define BUZZ_MAX_AMPLITUDE 600

void Buzz(int freq, int amplitude) { // zero for indefinite
  analogWriteFreq(freq);
  analogWrite(BUZZER_PIN, amplitude); // 0 to 1023 for PWM duty cycle
  //return millis();
}

void Buzz_Max() {
   Buzz_Delay (600, BUZZ_MAX_AMPLITUDE, 200);
}

void Buzz_Stop() {
   Buzz(200, 0);  // stop buzz
}

unsigned long Buzz_Delay(int freq, int amplitude, int time_delay) {
  Buzz(freq, amplitude);
  if (time_delay > 0) {
    delay(time_delay);
    Buzz_Stop();
  }
  return millis();
}
