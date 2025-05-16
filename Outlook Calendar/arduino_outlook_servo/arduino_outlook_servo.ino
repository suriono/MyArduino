#include <Servo.h>
#include "EasyNextionLibrary.h"

#define SERVO_PIN 9                // digital pin to servo signal

SoftwareSerial mySerial(10,11) ;   // RX,TX pins, the original library uses HardwareSerial
EasyNex myNex(mySerial);           // Go to my Instructible to convert to SoftwareSerial https://www.instructables.com/Nextion-Using-SoftwareSerial-Arduino/
Servo myservo;

char subject1[50];
char subject2[50];
char location1[50];
char location2[50];
long time1, time2, time1end;
boolean is_Dismiss = false;

// ======================= Setup ===========================================

void setup(){
   Serial.begin(9600);
   myNex.begin(9600); // Begin the object with a baud rate of 9600
   //myservo.attach(9); // servo signal pin
   delay(500);        // Wait for Nextion to start
   myNex.writeStr("tAppointment.txt", "Welcome to my cube"); 
   myNex.writeStr("tLocation.txt", "Uz is not here"); 
   myNex.writeNum("nTimeLeft.val", 0);      
}

// ======================= Loop ============================================

void loop(){

  static String cstr;

  myNex.NextionListen();

  if (Serial.available()) {
    char head;
    char cc[100] = "";  
    byte index=0;
    char ce;
    bool startcc = false;
    while (Serial.available()) {
      ce = Serial.read();
      Serial.println(ce);
      if (ce == '>') {  // beginning of the content
        cc[0] = '\0';
        cstr  = "";
        startcc = true;
      } else if (startcc) {
        if (ce == '<') {  // update the display
          switch (head) {
            case '1':                          // subject
              cstr.toCharArray(subject1, cstr.length()+1);  break; 
            case '2':                          // location
              cstr.toCharArray(location1, cstr.length()+1); break;
            case '3':                          // time
              time1 = atol(cc);   break; // time
            case '4':                          // time1 end
              time1end = atol(cc);   break; // time
            case '5':                          // subject
              cstr.toCharArray(subject2, cstr.length()+1);  break; 
            case '6':                          // location
              cstr.toCharArray(location2, cstr.length()+1); break;
            case '7':                          // time
              time2 = atol(cc);   break; // time
          }
          
        } else {
           cc[index++] = ce;
           cstr += ce;
        }
      } else if (!startcc) {
        head = ce;
      }
    }
    myNex.writeStr("tAppointment.txt", subject1); 
    myNex.writeStr("tLocation.txt", location1); 
    myNex.writeNum("nTimeLeft.val", time1);    
    if (time1 < 120 && time1 > -120) {
      servo_wave();        
    //} else {
     // myservo.write(0);
    }
  }
  
  delay(500);
}

// ====================== Dismiss button trigger ===========================

void trigger0() {
  myNex.writeStr("tAppointment.txt", "Next meeting ...");
  delay(2000);
  myNex.writeStr("tAppointment.txt", subject2); 
  myNex.writeStr("tLocation.txt", location2); 
  myNex.writeNum("nTimeLeft.val", time2); 

  // delay until the current appt ends
  unsigned long curtime = millis();
  while ( (millis() - curtime) < time1end*1000) {
    while (Serial.available()) {
      Serial.read();
    }
    // delay(time1end*1000); 
    delay(500);
  }
}

// ====================== Servo move back and forth ========================

void servo_wave() {
   myservo.attach(SERVO_PIN);
   delay(100);
   static int pos = 0  ;             // servo position
   for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  delay(100);
  myservo.detach();
}
