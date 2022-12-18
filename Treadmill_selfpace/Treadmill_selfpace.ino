//#include <SoftwareSerial.h>
#define MOTOR_PIN 5

unsigned long last_stop = millis();
//SoftwareSerial SerialSonar(5, 4); // RX and TX

void setup() {
  Serial.begin(57600);
  //TCCR0B = 0b00000010; // x1
  //TCCR0A = 0b00000011; // fast pwm
  //TCCR0B = 0b00000100; // x64
  //TCCR0A = 0b00000011; // phase correct
  //TCCR0B = 0b00000100; // x256
  //TCCR0A = 0b00000011; // fast pwm
  
//TCCR0B = 0b00000100; // x256
//TCCR0A = 0b00000001; // phase correct

//TCCR0B = 0b00000101; // x1024
//TCCR0A = 0b00000001; // phase correct


  pinMode(MOTOR_PIN, OUTPUT);
  Run_Motor(0);
  delay(3000);
}

void loop() {
  int distance = Get_Sonic_Distance();
  int motor_power = 0;
  bool is_motor_stop = false;
  //Serial.println(distance);

  if (distance > 5 && distance < 25) {
    //motor_power = 6000 / distance;
    //if (motor_power > 255) {
    //  motor_power = 255;
    //}
    Run_Motor(255);   // max power to reduce noise
    //Run_Motor(motor_power);
    last_stop = millis();
    is_motor_stop = false;
  } else {
    //Serial.println("====================STOP ==========");
    if ( (millis()-last_stop) > 200 ) {
       is_motor_stop = true;
       Run_Motor(0);
    }
  }
  Serial.print(distance); Serial.print(","); Serial.print(motor_power); Serial.print(","); Serial.println(is_motor_stop);
  delay(50);
}
