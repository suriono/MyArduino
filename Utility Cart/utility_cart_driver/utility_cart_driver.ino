

// Brake/clutch pins
#define enA         5     // PWM for the motor
#define potentioPin A2    // reading the potentiometer
#define LPWM        3     // PWM for braking

// Sensing max speed
#define SpeedBoosterPin_L     7     // goes LOW when enable max speed
#define SpeedBoosterPin_Input 8     // Pull-Up input to sense LOW/HIGH

#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards

#define DEBUG           // To serial print for debugging


// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

byte Speed_Init_Read;  // initial pedal reading = lowest analog read
                
void setup() 
{ 
   
  #ifdef DEBUG
      Serial.begin(115200);
      while (!Serial); // wait for Leonardo enumeration, others continue immediately
      // initialize device
      Serial.println(F("Initializing I2C devices..."));
  #endif



  pinMode(enA, OUTPUT) ; analogWrite(enA, 0);    // motor stop
  pinMode(LPWM, OUTPUT); analogWrite(LPWM,0);    // breake
  pinMode(potentioPin, INPUT);
  
  pinMode(SpeedBoosterPin_L, OUTPUT);
  pinMode(SpeedBoosterPin_Input, INPUT_PULLUP);
  
  digitalWrite(SpeedBoosterPin_L, HIGH); // initial disable max speed
 
  Serial.println("The motor is ready");
  Speed_Init_Read = analogRead(potentioPin);
  
}

// ================ LOOP =======================
#define POTENTIO_read_interval 10  // Reading the potentiometer interval in msec
#define BOOSTER_LIMIT 170           // potentio/pedal reading before enabling speed booster

void loop() {
  static unsigned long last_time_loop;
  static int last_PWMValue;
  static int last_breakValue;
  
  byte outPWM_motor;
  
  if (millis()-last_time_loop > POTENTIO_read_interval) {   // read motor potentiometer
    last_time_loop = millis();

    outPWM_motor = Potentio_to_PWM (analogRead(potentioPin));
    
    digitalWrite(SpeedBoosterPin_L, outPWM_motor < BOOSTER_LIMIT); // LOW=enable boost speed when potentioValue is high enough
    
    if (outPWM_motor > BOOSTER_LIMIT && !digitalRead(SpeedBoosterPin_Input)) { // booster is pressed
       outPWM_motor = 255;
       analogWrite(enA,  outPWM_motor);    // motor
       analogWrite(LPWM, outPWM_motor);    // brake

    } else if (outPWM_motor > 9 && outPWM_motor < last_PWMValue && outPWM_motor < 100) {
       analogWrite(enA,  outPWM_motor);    // motor
       analogWrite(LPWM, outPWM_motor + 20 + round(100/outPWM_motor));   // brake
    } else {    // normal reading
       analogWrite(enA,  outPWM_motor);    // motor
       analogWrite(LPWM, outPWM_motor);    // brake       
    }
    Serial.println(last_PWMValue);
    last_PWMValue = outPWM_motor;   
  }
  
}

// ======================================================
int Potentio_to_PWM (int potValue) {  // convert input potentio value to PWM
  int constrainvalue = constrain(potValue, Speed_Init_Read, 620);  // constraint the input value
  return map(constrainvalue, Speed_Init_Read, 620 , 0, 210); // 200 instead of 255 to limit speed;
}


// *********************** Convert Float to String with one decimal point *****
String floatToString(float number) {
   int decint = int(number*10)%10; // the first decimal
   return String(int(number)) + "." + String(decint);
}

