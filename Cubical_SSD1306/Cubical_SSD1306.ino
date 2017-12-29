#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "psl_logo.h"

#define SSD1306_i2c_address 0x3C // use I2C scanner to get address
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

String message[2];      // messages received from PC

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup()   {                
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, SSD1306_i2c_address);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.setTextColor(WHITE);
  // init done

  display.clearDisplay();
  //display.drawBitmap(0, 0,  bytearray, 114, 64, 1);
  display.drawBitmap(80, 24,  Smiley_picture, 32, 32, 1);
  display.display();
  delay(5000);

  message[0] = "Welcome";
  //message[0] = "My Outlook Calendar:";
  //message[1] = "Merry X-mas & Happy New Year. Enjoy your day !";
  //message[1] = "Merry Christmas";
  message[1] = "to Uz's cube";
  
}

// ===================== Loop ========================

void loop() {
  
  // draw scrolling text
  
  DisplayText(message);


  display.clearDisplay();
  display.drawBitmap(0, 0,  PSLLogo, 128, 64, 1);
  display.startscrollleft(0x00, 0x0F);
  display.display();
  delay(6700);
  display.invertDisplay(true);
  delay(1300); 
  display.stopscroll();
  display.invertDisplay(false);
  delay(1000);
}

// ======================= End of Loop ============================

// ********** incoming Serial ***********

void serialEvent() {
  static String TweetStr;
  int startind, stopind0, stopind1;

  String serialstr = Serial.readStringUntil('<');
      
  if (serialstr.indexOf("/") > 0) { //receive time verification
     stopind0 = TweetStr.indexOf("(1)");
     stopind1 = TweetStr.indexOf("(2)");
        
     message[0] = TweetStr.substring(0,stopind0);
        
     if (stopind1 > 0) {
        message[1] = TweetStr.substring(stopind0+3,stopind1);
     } else {
        message[1] = "";
     }
  } else {
     Serial.print(serialstr);  // send back for verification
     TweetStr = serialstr;
  }
  Serial.flush();
  while (Serial.available()) {
    Serial.read();  // read the remaining unread
  }

  
}

// ==================== Break Text ======================
String msg_display;
String msg_array[6];
int    msg_array_index = 0;
void BreakText(String inpstr) {
  int spaceindex = inpstr.indexOf(" ");
  //Serial.print("inpstr: ");
  //Serial.println(inpstr);
  if (spaceindex > 0) {
    //Serial.println(msg_display.length()+ spaceindex);
    if ( (msg_display.length()+ spaceindex) > 8 ) {
      //display.println(msg_display + inpstr.substring(0, spaceindex));
      msg_array[msg_array_index++] = msg_display;
      msg_display = inpstr.substring(0, spaceindex);
    } else if (msg_display.length() > 0) {
      msg_display += " " + inpstr.substring(0, spaceindex);
    } else {
      msg_display = inpstr.substring(0, spaceindex);
    }
    //msg_display += inpstr.substring(0, spaceindex);
    //Serial.print("=====");
    //Serial.println(msg_display);
    BreakText(inpstr.substring(spaceindex+1, inpstr.length()));
    
  } else if (inpstr.length()>0) {
    //Serial.print("last else: "); Serial.println(inpstr);
    if ( (msg_display.length()+ inpstr.length()) > 8) {
      //display.println(msg_display);
      msg_array[msg_array_index++] = msg_display;
      msg_display = inpstr.substring(0, spaceindex);
    } else if (msg_display.length() > 0) {
      msg_display += " " + inpstr.substring(0, spaceindex);
    } else {
      msg_display = inpstr;
    }
    //display.println(msg_display);
    msg_array[msg_array_index++] = msg_display;
    msg_display = "";
  }
}

// ==================== DisplayText ========================

void DisplayText(String str[]) {
  
  display.setTextSize(2);
  display.clearDisplay();
  display.setCursor(0,0);
  display.startscrollleft(0x00, 0x01);

  // get up to 6 rows of text
  msg_array_index = 0;
  for (int nn=0 ; nn < 6 ; nn++) {
    msg_array[nn] = ""; // reset
  }
  // Break words into six rows
  BreakText(str[1]);


  display.println(str[0]);
  for (int nn=0 ; nn < 3 ; nn++) {
    display.println(msg_array[nn]);
  }

  if (str[0].startsWith("Welcome")) {
    display.drawBitmap(96, 32,  Smiley_picture, 32, 32, 1);
  }
  
  display.display();
  delay(4000);

  if (msg_array[3].length() > 0) { // simplest text
    for (int16_t i=display.width()-1; i>=0 ; i-=1) {
      display.drawLine(i, display.height()/4, i, display.height()-1, BLACK);
      if ( (i%4) == 0 ) {
        display.display();
        delay(1);
      }
    }

    display.setCursor(0,17);
    for (int nn=3 ; nn < 6 ; nn++) {
      display.println(msg_array[nn]);
      display.display();
      delay(100);
    }
  }
  
  
  delay(4000);
  /*
  
  unsigned long last_scroll;

  static int starti = 0;
  String msg_str = str[0] + "     ";  // extra space for easy reading scroll

  int count_more_than_3_row = 0;
  for (int mm=0 ; mm < 50 ; mm++) {
    if ( (millis() - last_scroll) > 2000) {
      display.clearDisplay();
    } else {
      display.Uz_Clear_FirstRow();
    }
    display.setCursor(0,0);

    // top scroll message
    starti %= msg_str.length();
    int stopi = starti + 10;
    String tmpstr;
    if (stopi < msg_str.length()) {
      tmpstr = msg_str.substring(starti,stopi);
    } else {
      stopi %= msg_str.length();
      tmpstr = msg_str.substring(starti,msg_str.length()) + msg_str.substring(0,stopi) ;
    }
    display.println(tmpstr);
    // end of top scroll

    if ( (millis() - last_scroll) > 2000) {
      last_scroll = millis();

      if ((msg_array[3].length()+msg_array[3].length()+msg_array[3].length())>0) {
        if ((count_more_than_3_row %3) == 1) {
          for (int nn=3 ; nn < 6 ; nn++) {
            display.println(msg_array[nn]);
          }
        } else if ( (count_more_than_3_row % 3) == 0) {
          for (int nn=0 ; nn < 3 ; nn++) {
            display.println(msg_array[nn]);
          }
        } else {
          for (int16_t i=0; i<display.width(); i+=8) {
            display.drawLine(0, display.height()/4, i, display.height()-1, WHITE);
            display.drawLine(display.width()-1, display.height()/4, display.width()-i-1, display.height()-1, WHITE);
            display.display();
            delay(1);
          }
        }
        count_more_than_3_row++;
      } else {
        for (int nn=0 ; nn < 3 ; nn++) {
           display.println(msg_array[nn]);
        }
      }
    }
    
    display.display();
    delay(150);
    
    starti++;
  }
  */
  
  display.stopscroll();
}

