volatile unsigned long count1 = 0;
volatile unsigned long count2 = 0;

void motorStep1(){   // right side motor
 count1++;
}
void motorStep2(){   // left side motor
 count2++;
}

void setup() {
  Serial.begin(57600);
  
  attachInterrupt(digitalPinToInterrupt(2), motorStep1, RISING);
  attachInterrupt(digitalPinToInterrupt(3), motorStep2, RISING);
  Serial.println("Start");
  Serial.flush();
 
}  

void loop(){
   static unsigned long lasttime1=millis();
   static unsigned long lasttime2=millis();
   static unsigned long lastcount1 = 0;
   static unsigned long lastcount2 = 0;
   static float speedcount1;
   static float speedcount2;

   long lapsed = micros() - lasttime1;
    
   if ( lapsed > 200000) {
      
      noInterrupts(); // since count1 is a multiple byte value, must access atomically
        speedcount1 = (count1 - lastcount1)*1000000.0/ lapsed; 
        lastcount1 = count1;
        speedcount2 = (count2 - lastcount2)*1000000.0/ lapsed;// negative to differ from right
        lastcount2 = count2;
        lasttime1 = micros();
      interrupts();

      Serial.println(speedcount1);   // right motor, disconnect TX during uploading
      //Serial.print(speedcount1);   // right motor, disconnect TX during uploading
      if (speedcount2 == 0.0) {
         speedcount2 = 0.1;
      }
      //Serial.print(",");
      Serial.println(-speedcount2);   // left motor, negative
  }

 /*
  lapsed = micros() - lasttime2;
  if ( lapsed > 190000) {
      
      noInterrupts(); // since count1 is a multiple byte value, must access atomically
        speedcount2 = (count2 - lastcount2)*1000000.0/ lapsed;// negative to differ from right
        lastcount2 = count2;
        lasttime2 = micros();
      interrupts();

      //Serial.println(speedcount1);   // right motor, disconnect TX during uploading
      //Serial.print(speedcount1);   // right motor, disconnect TX during uploading
      if (speedcount2 == 0.0) {
         speedcount2 = 0.1;
      }
      Serial.print(",");
      Serial.println(-speedcount2);   // left motor, negative
  }
  */
}


