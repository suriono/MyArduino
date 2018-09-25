//#include <MegaServo.h>
#include <SabertoothSimplified.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

//#define PHONE_SERVO_PIN  2
//#define RPWM             11 // PWM A and used for trimmer

SoftwareSerial mySerial(9, 10); // RX, TX-> Sabertooth
//MegaServo Servos[1] ; // max servos is 32 for mega, 8 for other boards
SabertoothSimplified ST(mySerial); // Use SWSerial as the serial port.

//#define PHONE_SERVO_REF 90

unsigned long last_time, last_time1;
//boolean toggle_trimmer = false;   // toggle trimmer on and off

#define DEBUG


void setup()
{

  mySerial.begin(38400); 
  motorStop(); 
  mySerial.flush();

  #ifdef DEBUG
  Serial.begin(57600);
  #endif

  motorStop();

  /*
  //Servos[0].attach(PHONE_SERVO_PIN, 700, 1800);
  Servos[0].attach(PHONE_SERVO_PIN, 650, 1610);
  Servo_Phone(70); delay(1000);
  Servo_Phone(PHONE_SERVO_REF);
  */

  pinMode(A0, INPUT);       // to control maximum speed
  motorStop();
  // motorRun(40,180);
}

// ============== Loop =======================
void loop()   { 
  char inc;
  static String inpstr;
  static unsigned long lastimecurl;
  static boolean isValidCommand = false;

  if ( (millis() - lastimecurl) > 2000) { // clear input string if too long
    inpstr = "";
  }
  
  if (mySerial.available()) {
    int count = 0;
    isValidCommand = false;
    while (mySerial.available() && count < 50000) {
      count++;
      inc = mySerial.read();  // read the remaining bytes
      //Serial.println(inc);
      if (inc == '{') {
        inpstr = inc;
        lastimecurl = millis();
      } else if (inc == '}') {
        inpstr += inc;
        //Serial.print("promini robot input: "); Serial.println(inpstr);
        isValidCommand = true;
        break;
      } else {
        inpstr += inc;
      }
    }
    if (isValidCommand) {
      //Serial.print("promini input: "); Serial.println(inpstr);
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& jsonob = jsonBuffer.parseObject(inpstr);

      if (!jsonob.success()) {
        Serial.println("parseObject() failed");
      } else {
        int mag = jsonob["mag"];
        int angle = jsonob["angle"];
        int button = jsonob["button"];
        Serial.print("count: "); Serial.print(count);Serial.print(", mag: ");
        Serial.print(mag); Serial.print(", angle: ");
        Serial.print(angle); Serial.print(", button: ");
        Serial.println(button);
        
        motorRun( mag,  angle);
        last_time = millis();
      }    
    }  
    /*
    String instring;
    while(mySerial.available()) {
      char inchar = mySerial.read();
      if (inchar > 31) {
        instring += inchar;
      }
    }
    //Serial.println("Incoming msg:" + instring);

    if(instring.startsWith("anglespeed=")) {
      if (!setMotorSpeed(instring) ) {
        #ifdef DEBUG
        Serial.println("UNKNOWN motor command: " + instring);
        #endif
      }
    } else if (instring == "stop") {
      motorStop();
    } else if (instring.startsWith("servo=")) {
      setServo(instring);
    } else {
      #ifdef DEBUG
      Serial.println("UNKNOWN: " + instring);
      #endif
    }
    */
  }
  

  if ( (millis() - last_time) > 500) { // stop motor with 1 sec not getting a cmd
    #ifdef DEBUG
    Serial.print("No incoming instruction."); 
    #endif
    motorStop();
  }
} 

// ===============End of Loop ==================


// ========= Servo Phone =================
void setServo(String instring) {
  last_time = millis();
  String str_acceptable = "servo=";
  int pos = instring.substring(str_acceptable.length(), instring.length()).toInt(); 

  #ifdef DEBUG
  //Serial.print("Phone turn (+=right) : "); Serial.println(Servo_Phone(pos + PHONE_SERVO_REF));
  #endif
}

/*
int Servo_Phone(int pos) {
  Servos[0].write(pos);
  return Servos[0].read();
}
*/



// ==================================
unsigned long last_toggle_trimmer;
boolean setMotorSpeed(String instring) {
  int leftSpeed, rightSpeed;

  String str_acceptable = "anglespeed=";
  //if (instring == "stop") {
  //   motorStop();
  //   return true;
  //} else if(instring.startsWith(str_acceptable)) {
    last_time = millis();
    int anglespeed = instring.substring(str_acceptable.length(), instring.length()).toInt(); 

    int motor_pow = 0;
    int motor_degree = 0;
    if (anglespeed > 0) {
       motor_pow    = anglespeed & 127; // first 7 bits are power 0-127
       motor_degree = 3*((anglespeed >> 7) & 127); // next 7 bits are angle in 3 degree increment
    }
    #ifdef DEBUG
    Serial.print("Trimmer button press="); Serial.print(anglespeed>>14);
    #endif
      
    motorRun( motor_pow , motor_degree);

    /*
    if (anglespeed >> 14) {  // push down joystick for trimmer
       if ( (millis() - last_toggle_trimmer) > 1000) {
          toggle_trimmer = !(toggle_trimmer);
          last_toggle_trimmer = millis();
          if (toggle_trimmer) {
             analogWrite(RPWM, 120);  // NodeMCU max=1023 not 255
          } else {
             analogWrite(RPWM, 0);
          }
       }
    }
    */
    return true;
}
