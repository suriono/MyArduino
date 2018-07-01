volatile bool updated=false;
volatile unsigned long count1 = 0;

#define M1_INT 2

void motorStep1(){
count1++;
 updated = true;
}

void setup() {
  //detachInterrupt(0);
  Serial.begin(57600);
  //Serial.setTimeout(10000);
  
//  pinMode(13, OUTPUT);
//  pinMode(M1_SENS, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), motorStep1, RISING);
  Serial.println("Start");
  Serial.flush();
  //digitalWrite(13, HIGH);
}  

void loop(){
   static unsigned long tempCount;
   static unsigned long lasttime=millis();
   static boolean toggle = false;
   static unsigned long lastcount = 0;
   static float speedcount;

   long lapsed = micros() - lasttime;
    
   if ( lapsed > 20000) {
      
      noInterrupts(); // since count1 is a multiple byte value, must access atomically
        speedcount = (count1 - lastcount)*1000000.0/ lapsed; // (micros()-lasttime);
        lastcount = count1;
        lasttime = micros();
      interrupts();
      
      
      //if (speedcount > 0) {
        
        Serial.println(speedcount);
      //Serial.println(lastcount);
        
      //}
     
  }
}


