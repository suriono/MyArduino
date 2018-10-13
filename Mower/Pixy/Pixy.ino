#include <Pixy2.h>
#include <SabertoothSimplified.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <NewPing.h>

#define MAX_DISTANCE 254
#define MIN_SONIC_DIST_STOP 50  // minimum distance to stop the robot

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
    if (dist < 63 && dist > 10) {
       mag = dist*2;
       pixy.setLamp(1,0);
       Serial.print("Dist :"); Serial.println(String(dist));
    } else {
       pixy.setLamp(0,0);
    }
    
    pixy.ccc.getBlocks();
  
    if (pixy.ccc.numBlocks)
    {
      int wid = pixy.ccc.blocks[BLOCKN].m_width;
      int hei = pixy.ccc.blocks[BLOCKN].m_height;
      if ( (wid+hei) > 40 && (wid+hei) < 200 && max(wid/hei,hei/wid) < 3) {
        //pixy.ccc.blocks[i].print();
        int dx = pixy.ccc.blocks[BLOCKN].m_x - 160;
        Serial.print("dx: "); Serial.print(dx); Serial.print(",width:");
        Serial.print(wid); Serial.print(",");
        //Serial.print(hei); Serial.print(",");
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
