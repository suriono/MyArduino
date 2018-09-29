#include <SoftwareSerial.h>
SoftwareSerial mySerial(9, 12); // RX(not used), TX->D2(NodeMCU)

#define DEBUG

#define SMOOTH_COEF 0.5
#define TURN_COEF 2       // coefficient to slow turn, higher the slower

#define BUTTON_PRESS_PIN 10
int A0refy, A1refx;

void setup() {
  Serial.begin(57600);
  mySerial.begin(9600);
  pinMode(BUTTON_PRESS_PIN,INPUT);
  
  digitalWrite(BUTTON_PRESS_PIN,HIGH);   // press button
  A0refy = analogRead(A0);  // y-axis
  A1refx = analogRead(A1);  // x-axis
}

void loop() {
  static int last_smooth, last_mag;
  
  int A0ready = analogRead(A0)-A0refy;
  int A1readx = (analogRead(A1)-A1refx) / TURN_COEF;

  if (abs(A0ready) > 400) {   //when going fast
    A1readx /= 2;
  }

  //int xval = A1readx / 20; // map(A1readx, -520,520, -30,30);
  //if ( abs(A1readx) > 470) {
  //  xval = A1readx;
  //}

  //float xratio = sqrt(float(A1readx)*float(A1readx)) / 520.0  ;
  //int xval = ((A1readx>0)-(A1readx<0))*int((exp(xratio*TURN_COEF)-1.0)/(exp(TURN_COEF)-1.0) * 520.0);
  //Serial.print("A1readx :");Serial.print(A1readx); Serial.print(" ");
  //Serial.print("xval :"); Serial.println(xval);
  
  //int mag = map(max(abs(xval),abs(A1ready)), 0, 520,0,127);
  //int angle = atan2(xval,A1ready)*180.0/PI;
  int mag = map(max(abs(A0ready),abs(A1readx)), 0, 520,0,127);
  int angle = atan2(A0ready,A1readx)*180.0/PI;
  int button_press = digitalRead(BUTTON_PRESS_PIN);

  // make angle less sensitive
  
  last_mag = int( mag * SMOOTH_COEF + (1.0 - SMOOTH_COEF) * last_mag);

  //int send_angle = (((angle + 360) % 360) / 3) << 7; // increment by 3 degree then shift 7 bit
  //int send_angle_mag = (button_press << 14) + send_angle + last_mag; // send to NodeMCU

  #ifdef DEBUG
      //Serial.print("Button press="); Serial.print(button_press);
      //Serial.print(",Input  mag=") ; Serial.print(last_mag); 
      Serial.print(",angle=");     Serial.println(angle);
  #endif
  //Serial.print(send_angle_mag);  // actual number sent to NodeMCU
  //mySerial.print("joystick=" + String(send_angle_mag) + "<");
  String outstr = "{\"mag\":" + String(last_mag) + ",\"angle\":" + String(angle) + ",\"button\":" + String(button_press) +  '}';
  //Serial.println(outstr);
  mySerial.print(outstr); // + String(last_mag) + ',Angle:' + String(angle) + '}');
  delay(100);
}
