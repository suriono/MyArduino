void setup() {
 // Serial.begin(57600);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

}

void dim1(int pinout) {
  int anout[] = {0, 10, 13, 15, 24, 31, 36, 46, 53, 71, 79, 92, 119, 145, 170, 215, 255};
  for (int n = 0; n<17 ; n++) {
  //  Serial.println(anout[n]);
    analogWrite(pinout, anout[n]);    
    delay(20);
    if (n == 16) {
      delay(1000);
    }
  }
  for (int n = 16; n>=0 ; n--) {
  //  Serial.println(anout[n]);
    analogWrite(pinout, anout[n]);    
    delay(20);
    if (n == 16) {
      delay(1000);
    }
  }
}
void dim2(int blueratio, int greenratio) {
  int anout[] = {0, 10, 13, 15, 24, 31, 36, 46, 53, 71, 79, 92, 119, 145, 170, 215, 255};
  for (int n = 0; n<17 ; n++) {
  //  Serial.println(anout[n]);
    analogWrite(9, anout[n]/blueratio);  
    analogWrite(10, anout[n]/greenratio);        
    delay(20);
    if (n == 16) {
      delay(1000);
    }
  }
  for (int n = 16; n>=0 ; n--) {
  //  Serial.println(anout[n]);
    analogWrite(9, anout[n]/blueratio);
    analogWrite(10, anout[n]/greenratio);
    delay(20);
    if (n == 16) {
      delay(1000);
    }
  }
  
}

void loop() {
  dim1(9);
  dim1(10);
  
  dim2(1,8);
  dim2(1,1);
  dim2(8,1);
  //digitalWrite(2, HIGH);
  //delay(200);
  //digitalWrite(2, LOW);
  //delay(1000);
}
