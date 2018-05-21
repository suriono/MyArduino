#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "psl_logo.h"

#define SSD1306_i2c_address 0x3C // use I2C scanner to get address
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10

String header, calendar;      // messages received from PC
unsigned long Last_time_received_data;

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
  display.drawBitmap(80, 24,  Smiley_picture, 32, 32, 1);
  display.display();
  delay(2000);

  //header = "Waiting";
  //calendar = "for calender";
  
}

// ===================== Loop ========================

void loop() {

  static String st0, st1;

  if ( (millis() - Last_time_received_data) > 30000) { // not getting any input
    st0 = "Welcome";
    st1 = "to Uz's cube";
  } else if (header.length() > 1 && calendar.length() > 1) {
    st0 = header;
    st1 = calendar;
  }

  DisplayText(st0, st1);
  

  display.clearDisplay();
  display.drawBitmap(0, 0,  PSLLogo, 128, 64, 1);
  display.startscrollleft(0x00, 0x0F);
  display.display();
  delay(1000);
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  /*
  delay(6700);
  display.invertDisplay(true);
  delay(1300); 
  display.stopscroll();
  display.invertDisplay(false);
  
  static int nn=0;
  Serial.println(String(nn++) + " testing uz");
  Serial.flush();
  delay(5000);
  */
  Serial.println("Request"); // request a new one
  delay(1000);
}

// ======================= End of Loop ============================

// ********** incoming Serial ***********

void serialEvent() {
  byte startind, stopind0, stopind1;

  String serialstr = Serial.readStringUntil('<');

  //Serial.print(serialstr);

   stopind0 = serialstr.indexOf("(1)");
   stopind1 = serialstr.indexOf("(2)");

   if (stopind0 > 0 && stopind1 > 0) {
      Serial.println(serialstr);  // send back for verification
      
      header = serialstr.substring(0,stopind0);
      calendar = serialstr.substring(stopind0+3,stopind1);

      Last_time_received_data = millis();
     
   } else {
      Serial.println("ERROR");
   }
   //Serial.flush();

  while (Serial.available()) {
    Serial.read();  // read the remaining unread
  }
 
  
}

// ==================== Break Text ======================
String msg_display;
String msg_array[6];
byte   msg_array_index = 0;
void BreakText(String inpstr) {
  int spaceindex = inpstr.indexOf(" ");
  //Serial.print("inpstr: ");
  //Serial.print(inpstr); Serial.print("---------"); Serial.println(msg_array_index);
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
    //Serial.print("=====");
    //Serial.println(msg_display);
    BreakText(inpstr.substring(spaceindex+1, inpstr.length()));
    
  } else if (inpstr.length()>0) {
    //Serial.print("last else: "); Serial.println(inpstr);
    if ( (msg_display.length()+ inpstr.length()) > 8) {
      msg_array[msg_array_index++] = msg_display;
      msg_display = inpstr.substring(0, spaceindex);
    } else if (msg_display.length() > 0) {
       msg_display += " " + inpstr.substring(0, spaceindex);
    } else {
      msg_display = inpstr;
    }
    //Serial.print("last else msg_display: "); Serial.println(msg_display);
    msg_array[msg_array_index++] = msg_display;
    msg_display = "";
  }
}

// ==================== DisplayText ========================

void DisplayText(String str0, String str1) {
//void DisplayText() {
  //String str0, str1;

  //str0 = header;
  //str1 = calendar;
  
  display.setTextSize(2);
  display.clearDisplay();
  display.setCursor(0,0);
  display.startscrollleft(0x00, 0x01);

  // get up to 6 rows of text
  msg_array_index = 0;
  for (byte nn=0 ; nn < 6 ; nn++) {
    msg_array[nn] = ""; // reset
  }
  // Break words into six rows
  BreakText(str1);

  //Serial.println("messages: ");
  //for (byte nn=0 ; nn < 6 ; nn++) {
   // Serial.println(msg_array[nn]);
  //}
  //Serial.println("-------------------------");


  display.println(str0);
  //Serial.println("First 3 rows:");
  for (byte nn=0 ; nn < 3 ; nn++) {
    display.println(msg_array[nn]);
    //Serial.println(msg_array[nn]);
  }

  if (msg_array[1].length()<9 && msg_array[2].length()<9) {
    display.drawBitmap(96, 32,  Smiley_picture, 32, 32, 1);
  } else if ( msg_array[1].length()<9 && msg_array[0].length()<9) {
    display.drawBitmap(96, 16,  Smiley_picture, 32, 32, 1);
  }
  
  display.display();
  delay(3000);

  if (msg_array[3].length() > 0) { // more than 3 rows
    for (int i=display.width()-1; i>=0 ; i--) {
      display.drawLine(i, display.height()/4, i, display.height()-1, BLACK);
      if ( (i%4) == 0 ) {
        display.display();
        delay(1);
      }
    }

    display.setCursor(0,17);
    for (byte nn=3 ; nn < 6 ; nn++) {
      display.println(msg_array[nn]);
      delay(1);
    }

    if (msg_array[4].length()<9 && msg_array[5].length()<9) {
      display.drawBitmap(96, 32,  Smiley_picture, 32, 32, 1);
    } else if ( msg_array[4].length()<9 && msg_array[3].length()<9) {
      display.drawBitmap(96, 16,  Smiley_picture, 32, 32, 1);
    }
    
    display.display();
    delay(4000);
  }
  
  delay(4000);
 
  
  display.stopscroll();
}

