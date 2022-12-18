#include <NewPing.h>
#define ECHO_PIN 12
#define TRIG_PIN 10
#define MAX_DISTANCE 400

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

int Get_Sonic_Distance() {
  delay(1);
  int tmp_distance = int(sonar.ping_median(5)/50); // in cm
  //Serial.println(tmp_distance);
   
  if (tmp_distance > 0) {
    return tmp_distance;
  }  
  return 400;   // assuming it's close when it's too far away 
}
