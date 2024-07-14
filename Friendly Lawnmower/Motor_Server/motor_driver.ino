#define FORWARD_PIN    16
#define REVERSE_PIN    14
#define RIGHT_PIN      12
#define LEFT_PIN       13
#define MOW_C_PIN      15
#define MOW_BLADE_PIN  2

// ================ Motor Initialize ================================

void Motor_Init() {
   pinMode(FORWARD_PIN, OUTPUT);
   pinMode(REVERSE_PIN, OUTPUT);
   pinMode(LEFT_PIN, OUTPUT);
   pinMode(RIGHT_PIN, OUTPUT);
   pinMode(MOW_C_PIN, OUTPUT);
   pinMode(MOW_BLADE_PIN, OUTPUT);
   delay(500);
   Motor_Stop();
   delay(1000);
}

// ================ Motor Stop =====================================

void Motor_Stop() {
   digitalWrite(FORWARD_PIN, HIGH);
   digitalWrite(REVERSE_PIN, HIGH);
   digitalWrite(LEFT_PIN, HIGH);
   digitalWrite(RIGHT_PIN, HIGH);
   digitalWrite(MOW_C_PIN, HIGH);
   digitalWrite(MOW_BLADE_PIN, HIGH);
}

// ================ Motor Forward ===================================

void Motor_Forward(unsigned long delaytime, unsigned long afterdelay) {
   digitalWrite(FORWARD_PIN, LOW); delay(delaytime); digitalWrite(FORWARD_PIN, HIGH);
   delay(afterdelay);
}

// ================ Motor Reverse ===================================

void Motor_Reverse(unsigned long delaytime, unsigned long afterdelay) {
   digitalWrite(REVERSE_PIN, LOW); delay(delaytime); digitalWrite(REVERSE_PIN, HIGH);
   delay(afterdelay);
}

// ================ Motor Left ===================================

void Motor_Left(unsigned long delaytime, unsigned long afterdelay) {
   digitalWrite(LEFT_PIN, LOW); delay(delaytime); digitalWrite(LEFT_PIN, HIGH);
   delay(afterdelay);
}

// ================ Motor Right ===================================

void Motor_Right(unsigned long delaytime, unsigned long afterdelay) {
   digitalWrite(RIGHT_PIN, LOW); delay(delaytime); digitalWrite(RIGHT_PIN, HIGH);
   delay(afterdelay);
}

// ================ Mow Blade ===================================

void Motor_Mow(unsigned long delaytime, unsigned long afterdelay) {
   digitalWrite(MOW_C_PIN, LOW); delay(1000); digitalWrite(MOW_BLADE_PIN, LOW);
   delay(delaytime); digitalWrite(MOW_C_PIN, HIGH); digitalWrite(MOW_BLADE_PIN, HIGH);
   delay(afterdelay);
}
