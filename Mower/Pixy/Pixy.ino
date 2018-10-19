#include <Pixy2.h>
#include <SabertoothSimplified.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <NewPing.h>

#define MAX_DISTANCE        254
#define MIN_SONIC_DIST_STOP 50  // minimum distance to stop the robot
#define EVA_PIN             6   // sonic detect a close object, could be Eva picture

SoftwareSerial mySerial(9, 10); // RX(not used), TX->Sabertooth
SabertoothSimplified ST(mySerial); // Use SWSerial as the serial port.
NewPing sonar(7, 8, MAX_DISTANCE); // trig, echo, max distance

Pixy2 pixy;

#define DEBUG

#define BLOCKN 0      // the stored block number 0-6
unsigned long last_time;

void setup()
{
  mySerial.begin(38400);
  motorStop(); 
  mySerial.flush();
  pinMode(A0, INPUT);       // to control maximum speed
  pinMode(EVA_PIN, OUTPUT);

  #ifdef DEBUG
    Serial.begin(57600);
  #endif
  
  Serial.print("Starting...\n");
  pixy.init();
  motorStop();
  pixy.setLamp(0,0);
}

// ==================== Loop =================================

void loop()
{ 
  char inc; 
  static String inpstr;
  static unsigned long lastimecurl;
  static boolean isValidCommand = false;

  if (mySerial.available()) {
      if ( (millis() - lastimecurl) > 2000) inpstr = ""; // clear after 2 sec
      
      int count = 0;
      //isValidCommand = false;
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
        isValidCommand = false;
        //Serial.print("promini input: "); Serial.println(inpstr);
        StaticJsonBuffer<200> jsonBuffer;
        JsonObject& jsonob = jsonBuffer.parseObject(inpstr);

        if (!jsonob.success()) {
          Serial.println("parseObject() failed");
        } else {
          int mag = jsonob["mag"];
          int angle = jsonob["angle"];
          int button = jsonob["button"];
          //Serial.print("count: "); Serial.print(count);Serial.print(", mag: ");
          //Serial.print(mag); Serial.print(", angle: ");
          //Serial.print(angle); Serial.print(", button: ");
          //Serial.println(button);
          motorRun( mag,  angle);
          last_time = millis();
        }    
      }  
      
  } else {  // when no incoming serial from nodeMCU

    int dist = Get_Sonic_Distance();
    int mag = 0;   
    if (dist < 63 && dist > 5) {
       mag = dist*2;
       //pixy.setLamp(1,0);
       digitalWrite(EVA_PIN, LOW);
       Serial.print("Dist :"); Serial.print(String(dist));
    } else {
       digitalWrite(EVA_PIN, HIGH);
       pixy.setLamp(0,0);
    }
    
    pixy.ccc.getBlocks();

    //Serial.print("block num:"); Serial.println(pixy.pirnt());
  
    if (pixy.ccc.numBlocks) {
      byte biggest_block = 0;
      /*
      int widest = 0;
      for (byte nn = 0 ; nn < pixy.ccc.numBlocks; nn++) { // find biggest one
         if ( pixy.ccc.blocks[nn].m_width > widest) {
            widest = pixy.ccc.blocks[nn].m_width;
            biggest_block = nn;
         }
      }
      */
      
      int wid = pixy.ccc.blocks[biggest_block].m_width;
      int hei = pixy.ccc.blocks[biggest_block].m_height;
      if ( wid > 70 && hei > 70 && (hei/wid) < 1) { //  && max(wid/hei,hei/wid) < 3) {
        //pixy.ccc.blocks[i].print();
        int dx = pixy.ccc.blocks[biggest_block].m_x - 160;
        //Serial.print(",block number: "); Serial.print(pixy.ccc.numBlocks);
        Serial.print(",dx: "); Serial.print(dx); Serial.print(",width:");
        Serial.print(wid); Serial.print(",height:");
        Serial.print(hei); Serial.print(",");
        //Serial.print(" X : ");Serial.print(pixy.ccc.blocks[i].m_x); 
        //Serial.print(", Y :");Serial.print(pixy.ccc.blocks[i].m_y); 
        //Serial.print(",");
        //Serial.print(pixy.ccc.blocks[i].m_angle); Serial.print(",");
        //Serial.println();
        
        
        //int mag   = min((120-wid)*(wid<70), 100);
        
        int angle = 90 - dx/3;
        //String outstr = "{\"mag\":" + String(mag) + ",\"angle\":" + String(angle) + ",\"button\":" + String(button_press) +  '}';
        //String outstr = "{\"mag\":" + String(mag) + ",\"angle\":" + String(angle) +  '}';
        //Serial.println(outstr);
        //mySerial.print(outstr);
        
        motorRun(mag , angle);
        
        delay(200);
        last_time = millis();
      } 
    }
  }  

  if ( (millis() - last_time) > 300) { // stop motor with 1 sec not getting a cmd
    //#ifdef DEBUG
    //Serial.print("No instruction."); 
    //#endif
    motorStop();
  }
}
