
void Light() {
  static unsigned long last_time_light;
  static byte fade_count = 0;
  static bool fade_inout = true;
  static bool toggle_light = true;
  if ( (millis() - last_time_light) > FADE_DELAY ) {
    last_time_light = millis();

    if (fade_count == 0) {
      fade_inout = true;
    } else if (fade_count == 200) {
      fade_inout = false;
    }

    if (fade_inout) {
      fade_count += 5;
    } else {
      fade_count -= 5;
      if (fade_count == 0) { // switch light
        toggle_light = !toggle_light;
      }
    }

    if (toggle_light) {
      
      analogWrite(LPWM_PIN,0);
      analogWrite(RPWM_PIN,fade_count);
    } else {
      analogWrite(RPWM_PIN,0);
      analogWrite(LPWM_PIN,fade_count);
    }
    
  }
  
}
