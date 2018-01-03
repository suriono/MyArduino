#include <LiquidCrystal.h> 
#include <DFR_Key.h>      // required for the LCD shield

// LiquidCrystal lcd(rs,reset,d4,d5,d6,d7); // define our LCD and which pins to user
//LiquidCrystal lcd(4,5,6,7,8,9); // define our LCD and which pins to user to Arduino
//Pin assignments for SainSmart LCD Keypad Shield
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 
// None=0, Select=1, Left=2, Up=3, Down=4, Right=5
DFR_Key keypad;

String str0 = "";
String str1 = "";
String str2 = "";
String str3 = "";
String timestr = "";
String serialstr,TweetStr;
byte smiley[8] = {B00000,B10001,B00100,B00000,B10001,B01110,B00000,B00000}; 

void setup() {
  lcd.createChar(1, smiley);   // create memory space for smiley in the LCD
  lcd.begin(16, 2); // need to specify how many columns and rows are in the LCD unit
  Serial.begin(9600);    
  //Serial.setTimeout(10000);
   // keypad.setRate(10);  // optional rate setting
}

void loop() {
  getKeyPad();

  if (str0.length()) {
    noInterrupts();
    printlcd(str0,str1, 100);
    interrupts();
    getKeyPad();
    delay(1000);   // to allow interruption of serial reading
    getKeyPad();
    delay(1000);
    getKeyPad();
    delay(1000);
    getKeyPad();
    if(str2.length()) {
      noInterrupts();
      printlcd(str2,str3, 100);
      interrupts();
      getKeyPad();
    }
    delay(1000);
    getKeyPad();
    delay(1000);
    getKeyPad();
    printlcd("Checking Twitter","Outlook Calendar",100);
    delay(1000);
    getKeyPad();
    delay(1000);
    getKeyPad();
  } else {
    printlcd("Waiting for", "Twitter or", 2000);
    getKeyPad();
    printlcd("Outlook update", "Please wait ...", 2000);
  }
}

// ********** Key Pad **********************
void getKeyPad() {
  int localKey = 0;
  localKey = keypad.getKey();
  if (localKey == 1) { // select key
    printlcd("My phone number:", "952-876-3050", 10000);
  } else if (localKey==2) { // left key
    printlcd("Email: surionou@", "polarsemi.com", 10000);
  }
}

// ********** incoming Serial ***********
void serialEvent() {
  int startind, stopind0, stopind1, stopind2, stopind3;

  serialstr = Serial.readStringUntil('<');
      //delay(10);
  if (serialstr.indexOf("/") > 0) { //receive time verification
        timestr = serialstr;
        stopind0 = TweetStr.indexOf("(1)");
        stopind1 = TweetStr.indexOf("(2)");
        stopind2 = TweetStr.indexOf("(3)");
        stopind3 = TweetStr.indexOf("(4)");
      
        str0 = TweetStr.substring(0,stopind0);
        
        if (stopind1 > 0) {
          str1 = TweetStr.substring(stopind0+3,stopind1);
        } else {
          str1 = "";
        }
        
        if (stopind2 > 0) {
          str2 = TweetStr.substring(stopind1+3,stopind2);
        } else {
          str2 = "";
        }
        if (stopind3 > 0) {
          str3 = TweetStr.substring(stopind2+3,stopind3);
        } else {
          str3 = "";
        }
        //Serial.println("LCD is updated");
        delay(10);
  } else if(serialstr == "NoTweet") {
        // No update to LCD
        //Serial.println("No update to LCD");
        delay(10);
  } else if(serialstr == "Reset(1)") {
        Serial.flush();
        delay(1000);
        Serial.end();
        delay(1000);
        Serial.begin(9600);
        delay(1000);
  } else {
        Serial.print(serialstr);  // send back for verification
        //Serial.flush();
        TweetStr = serialstr;
  }
  Serial.flush();
  while (Serial.available()) {
    Serial.read();  // read the remaining unread
  }
}

//************************** LCD output *********************
void printlcd(String strone, String strtwo, unsigned long delaytime) {
  int colonindex;
  lcd.clear();
  
  lcd.setCursor(0,0); // positions starting point on LCD, column 0, row 0 (that is, the top left of our LCD)
  //strone = strone.substring(0,15);
  colonindex = strone.indexOf(";");
  strone.replace(";", " ");
  lcd.print(strone);
  if (colonindex > -1) {
    lcd.setCursor(colonindex,0); 
    lcd.write(1);
  }
  lcd.setCursor(0,1); // positions starting point on LCD, column 0, row 0 (that is, the top left of our LCD)
  colonindex = strtwo.indexOf(";");
  strtwo.replace(";", " ");
  lcd.print(strtwo);
  if (colonindex > -1) {
    lcd.setCursor(colonindex,1); 
    lcd.write(1);
  }
  if (delaytime>0) {
    delay(delaytime);
  }
}

