//int Joystick_Pin = A0;
//int Joystick_read;
int A0ref, A1ref;

void setup() {
  Serial.begin(9600);
  pinMode(8,INPUT);
  
  digitalWrite(8,HIGH);
  A0ref = analogRead(A0);
  A1ref = analogRead(A1);
  //pinMode(7,INPUT);
}

void loop() {
  //Serial.print("A0=,LR=");
  int A0read = analogRead(A0)-A0ref;
  //Serial.print(A0read);
  //Serial.print(",");
  //Serial.print("A1,FB=");
  int A1read = analogRead(A1)-A1ref;
  //Serial.print(A1read);
  //Serial.print(",");
  //Serial.print("Press down = ");
  //Serial.print(digitalRead(8));
  int mag = map(max(abs(A0read),abs(A1read)), 0,1023,0,127);
  //analogWrite(5, mag);
  Serial.print(mag);
  Serial.print(",");
  int angle = atan2(A0read,A1read)*180.0/PI;
  Serial.print(angle);
  Serial.print(",");
  int send_angle = (((angle + 360) % 360) / 3) << 7; // increment by 3 degree then shift 7 bit
  Serial.print(send_angle); Serial.print(",");
  Serial.print((send_angle>>7)*3); Serial.print(",");
  Serial.println(send_angle & 127);
  delay(500);
}

