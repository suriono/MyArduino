#include <NewPing.h>

//#define DEBUG

#define TRIGGER_PIN  11  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     10  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
}

void loop() {
  if (is_Uz_in_office()) {
    #ifdef DEBUG
    Serial.println("Uz is in office (within the last 5 sec)");
    #endif
    Serial.println("in_office");
  //} else {
   // Serial.println("out_office");
  }
  delay(5000);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
}
// =================== End of LOOP ========================

// =================== is Uz at the office the last 60 secs ? ============

boolean is_Uz_in_office() {
  static unsigned long last_seen;
  
  int dist_cm = sonar.ping_median(11)*0.034/2.0;

  if (dist_cm < 125) {              // appear in the office
    last_seen = millis();
  }

  #ifdef DEBUG
  Serial.print("======Last seen: "); 
  Serial.print((millis()-last_seen)/1000); Serial.print(" sec ago, ");
  Serial.print(dist_cm); Serial.println("cm");
  #endif

  return ((millis()-last_seen)/1000) < 4;   // seen within the last 60 sec 
}

