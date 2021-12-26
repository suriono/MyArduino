
const int VRxPin = A0;
const int OutV = A2;
#define PWMpin 6
#define PressPin 7
#define BreakPin 3

int VRx = 0;        // value read from the horizontal pot

void setup() {
  Serial.begin(57600);
  pinMode(PressPin,INPUT_PULLUP);
  
  while (abs(VRx - 511)>10 || PWM_Out() > 5) {
      
    VRx = analogRead(VRxPin);
    Serial.print(VRx); Serial.println("Waiting for joystick to be centered");
    delay(200);
  }
}

void loop() {

  int X_PWM = PWM_Out(); // int(volt_input * 100.0) / 3;  // limit the speed
  
  analogWrite(PWMpin, X_PWM);           // output PWM
  int PressButton = digitalRead(PressPin);

  if (PressButton) {
    digitalWrite(BreakPin, LOW);
  } else {
    digitalWrite(BreakPin, HIGH);
  }

  // print the results to the Serial Monitor:
  Serial.print("VRrx = "); Serial.print(VRx);
  Serial.print("\tX PWM = "); Serial.print(X_PWM);
  Serial.print("\tOut analog = "); Serial.print(analogRead(OutV));
  Serial.print("\tPress Button = "); Serial.print(PressButton);
  Serial.println();

  delay(200);
}

// ===========================
int PWM_Out() {
  VRx = analogRead(VRxPin);
  float volt_input = abs(VRx - 512) * 3.3/1023.0;
  return int(volt_input * 250.0/3.3);  // limit the speed
}
