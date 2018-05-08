#define LRPin A0
#define FBPin A1
//int Joystick_read;

void setup() {
  Serial.begin(57600);
}

void loop() {
  //int LRVolt = analogRead(A0)*5.0/1023; // in voltage
  int LRmag = analogRead(LRPin)-512;
  int FBmag = analogRead(FBPin)-512;
  //Serial.print(">Mag=" + String(LRmag*LRmag+FBmag*FBmag) + "<"); 
  
  if (LRmag*LRmag+FBmag*FBmag > 10000) {
     int angle = int(atan2(LRmag,FBmag)*180.0/PI);
  
     Serial.print(">A=" + String(angle) + "<");
     Serial.flush();
     //Serial.println("LR=" + String(LRmag));
     //Serial.println("FB=" + String(FBmag));
  } else {
     Serial.print(">Stop<");
     Serial.flush();
  }
  delay(400);
}

