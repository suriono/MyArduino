

void Run_Motor(byte value) {
  analogWrite(MOTOR_PIN , value);
}

/*
/ Pins D5 and D6 are 62.5kHz
TCCR0B = 0b00000001; // x1
TCCR0A = 0b00000011; // fast pwm
// Pins D5 and D6 - 31.4 kHz
TCCR0B = 0b00000001; // x1
TCCR0A = 0b00000001; // phase correct
// Pins D5 and D6 - 7.8 kHz
TCCR0B = 0b00000010; // x8
TCCR0A = 0b00000011; // fast pwm
// Pins D5 and D6 - 4 kHz
TCCR0B = 0b00000010; // x8
TCCR0A = 0b00000001; // phase correct
// Pins D5 and D6 - 976 Hz - default
TCCR0B = 0b00000011; // x64
TCCR0A = 0b00000011; // fast pwm
// Pins D5 and D6 - 490 Hz
TCCR0B = 0b00000011; // x64
TCCR0A = 0b00000001; // phase correct
// Pins D5 and D6 - 244 Hz
TCCR0B = 0b00000100; // x256
TCCR0A = 0b00000011; // fast pwm
// Pins D5 and D6 - 122 Hz
TCCR0B = 0b00000100; // x256
TCCR0A = 0b00000001; // phase correct
// Pins D5 and D6 - 61 Hz
TCCR0B = 0b00000101; // x1024
TCCR0A = 0b00000011; // fast pwm
// Pins D5 and D6 - 30 Hz
TCCR0B = 0b00000101; // x1024
TCCR0A = 0b00000001; // phase correct
*/
