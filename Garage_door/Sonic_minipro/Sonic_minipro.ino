#include <NewPing.h>
#include <SoftwareSerial.h>

SoftwareSerial SerialSonar(8, 13); // RX TX only transmit 13 needed

// 3.3V MiniPro

#define ECHO_PIN 12
#define TRIG_PIN 10
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

// ==========================================================

void setup() {
  Serial.begin(9600);
  SerialSonar.begin(9600);
 
}

// ==========================================================

void loop() {
  int dist = Get_Sonic_Distance();
  Serial.println(dist);
  SerialSonar.print(dist);
  delay(9150);
}

// ==========================================================

// ============= Take ultra sonic distance in cm =================
int Get_Sonic_Distance() {
  for (int nn = 0 ; nn < 10; nn++) {
    int tmp_distance = int(sonar.ping_median(21)/50); // in cm
    if (tmp_distance > 0) {
      return tmp_distance;
    }
    delay(100);
  }
  return 400;   // assuming it's close when it's too far away
}
