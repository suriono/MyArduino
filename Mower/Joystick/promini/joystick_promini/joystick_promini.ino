#include <SoftwareSerial.h>
SoftwareSerial mySerial(9, 12); // RX(not used), TX->D2(NodeMCU)

#define DEBUG

#define SMOOTH_COEF 0.2

#define BUTTON_PRESS_PIN 10
int A0ref, A1ref;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(BUTTON_PRESS_PIN,INPUT);
  
  digitalWrite(BUTTON_PRESS_PIN,HIGH);   // press button
  A0ref = analogRead(A0);  // x-axis
  A1ref = analogRead(A1);  // y-axis
}

void loop() {
  static int last_smooth, last_mag;
  int A0read = analogRead(A0)-A0ref;
  
  int A1read = analogRead(A1)-A1ref;
  int mag = map(max(abs(A0read),abs(A1read)), 0, 520,0,127);
  int angle = atan2(A0read,A1read)*180.0/PI;
  int button_press = digitalRead(BUTTON_PRESS_PIN);

  // make angle less sensitive
  /*
  if (abs(90-angle) < 75) {
     angle = angle + (90 - angle)* 2 / 3;
  } else if (abs(90+angle) < 60) {
     angle = -90 - (angle + 90) / 3;
  } else if (angle == 180) {
     angle = 0;
  } else if (angle == 0) {
     angle = 180;
  }
  */

  last_mag = int( mag * SMOOTH_COEF + (1.0 - SMOOTH_COEF) * last_mag);

  int send_angle = (((angle + 360) % 360) / 3) << 7; // increment by 3 degree then shift 7 bit
  int send_angle_mag = (button_press << 14) + send_angle + last_mag; // send to NodeMCU

  #ifdef DEBUG
      //Serial.println("");
      Serial.print("Button press="); Serial.print(button_press);
      Serial.print(",Input  mag=") ; Serial.print(last_mag); 
      Serial.print(",angle=");     Serial.println(angle);
      //Serial.print("Output mag=");
      //Serial.print(send_angle_mag & 127); Serial.print(",angle=");
      //Serial.print( ((send_angle_mag>>7) & 127)*3 ); Serial.print(",button=");
      //Serial.println( send_angle_mag >> 14);
  #endif
  //Serial.print(send_angle_mag);  // actual number sent to NodeMCU
  mySerial.print("joystick=" + String(send_angle_mag) + "<");
  delay(100);
}

