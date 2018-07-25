

unsigned long Buzz(int freq, int amplitude) {
  analogWriteFreq(freq);
  analogWrite(BUZZER_PIN, amplitude); // 0 to 1023 for PWM duty cycle
  return millis();
}

void Buzz_Max() {
   Buzz (300, 300);
}

void Buzz_Stop() {
   Buzz(200, 0);  // stop buzz
}

