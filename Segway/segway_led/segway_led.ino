#define LED_PIN     3      // RPWM
#define LED_OFFSET  50     // out of 255 for lowest PWM

#define DEBUG

void setup() {
  #ifdef DEBUG
    Serial.begin(115200);
  #endif
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);
}

void loop() {
  static unsigned long incr = 0;
 
  int led_mag = map(int(125*(1.0 + sin(incr/40.0))),0,250, LED_OFFSET,255);
  analogWrite(LED_PIN, led_mag);
  
  incr++;
  delay(10);

  #ifdef DEBUG
    Serial.println(led_mag);
  #endif

}
