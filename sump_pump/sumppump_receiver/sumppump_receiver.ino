
#include <LiquidCrystal.h> 
LiquidCrystal lcd(13,12,11,10,9,8);    // reset,enable,d4,d5,d6,d7
byte smiley[8] = {B00000,B10001,B00100,B00000,B10001,B01110,B00000,B00000};

#define Buzzer_pin 4       // D4 for buzzer
#define Dist_water_min 18  // The minimum water distance to sensor to trigger buzzer
#define Dist_wait 21       // Wait distance until discharge

String toprowstr;
int counting=0; //, count_web;
byte Newdata,Lastdata;
float Avgdata;
#define SCREEN_UPDATE 10000            // LED update in milliseconds
#define MAX_DISTANCE 50
#define MIN_WEB_UPDATE_TIME 10          // Minimum in minutes web update interval
#define DATA_SIZE 60
float DataArr[DATA_SIZE];
//={22.8,21.6,31.0,29.5,28.3,26.7,25.2,23.8,22.2,20.7,27.0,31.0,29.5,28.2,27.1,25.7,24.4,23.2,21.9,20.8,31.0,31.1,29.5,28.4,27.1,25.8,24.5,23.3,21.9,20.8,28.0,29.5,28.2,26.8,25.3,24.1,22.7,21.3,31.0,29.6,28.1,26.9,25.5,24.2,22.9,21.6,30.0,29.6,28.3,27.0,25.7,24.4,23.1,21.6,31.0,31.0,29.4,28.3,27.0,25.7,24.4,23.1,21.8,20.7,30.0,29.5,28.2,26.8,25.3,24.0,22.6,21.2,30.0,29.6,28.5,27.3,26.1,24.8,23.6,22.4,21.2,28.0,29.6,28.6,27.4,26.1,24.9,23.8,22.5,21.3,30.0,29.7,28.3,27.0,25.6,24.2,22.9,21.4,25.0,30.0};

unsigned long Last_discharge, Last_web_update;  // last discharged in milliseconds
#define DISCHARGE_ARRAY 5
unsigned long Discharges[DISCHARGE_ARRAY];
//={45,47,48,49,48}; // last five discharges in minutes
#define INTERVAL_PIN 5                // Analog PIN for potentiometer to adjust interval
//int interval_data, interval_web;      // website update in minutes

void setup() {
  Serial.begin(57600); // initialize the LED pin as an output:

  lcd.createChar(1, smiley);   // create memory space for smiley in the LCD
  lcd.begin(16, 2); // need to specify how many columns and rows are in the LCD unit
  Serial.flush();
  printlcd("Initial distance", "test ...",1000);
  for(int n=8; n>0; n--) { // initial distance sensing
     Serial.print(">Send_distance<");
     Serial.flush();
     //interval = Get_interval(); 
     toprowstr = "Interval:" + String(Get_interval_web()) + ",C=" + String(n);
     delay(1000);
     if(Serial.available()) {
       //serialEvent();
       readSerial();
     }
     delay(2000);
  }
  Last_discharge = millis(); // - 70*60000;  // last term for initial time in msec
  add_depth_array();  // add average data
  update_SD();
  //Serial.print(">SD_comma<");   // get the number of commas from SD file

  //send_to_Web();  // initial data for previous data, next time take it from the client SD JUMP1.js file
  
  Lastdata = Newdata;
  //counting = 0;
  //Last_discharge = millis() + 60*1000;  // the 2nd term when re-program for initial time
  //printlcd("Start the main", "program ...", 1000);
}

void loop() {
  //unsigned long tmptime=millis();
  if (Serial.available() <= 0) { // no incoming Serial data
     if ( (Newdata-Lastdata)>3 || counting >= Get_interval_data()*(60000/SCREEN_UPDATE) ) { // pump discharged or interval
        //if (abs(Newdata-Lastdata)>0) { // for testing discharge purpose
        if ((Newdata-Lastdata)>3) {
           Avgdata = Newdata;
           if ( Get_lastdischarge() > 1) { // in case last one was in the middle of discharging
              printlcd("Discharging","oldD:"+String(Lastdata)+",newD:"+String(Newdata), 5000);
              add_discharge_array(); // add Last_discharge to the 2nd last index, the last index for ongoing
              //delay(5000);
           }
        }
        add_depth_array();  // add average data 
        Lastdata = Newdata;
     } else if (Newdata > Dist_wait && (millis()-Last_web_update)/60000 > Get_interval_web()) {
        //send_to_Web();      // only uses data in the for depth
        update_SD();
        //Serial.print(">SD_comma<");   // get the number of commas from SD file
     } else {
        Lastdata = Newdata;
        Serial.print(">Send_distance<");
        Serial.flush();
        // E:Elapsed in minutes, C: counts
        toprowstr = "E="+String(Get_lastdischarge())+",I=" + String(Get_interval_web()) ;
        //Lastdata = Newdata;
     }
     delay(SCREEN_UPDATE);
  } else {
     readSerial(); 
  }
}

// ******** Get elapsed since the last discharge in minutes *****
int Get_lastdischarge() {   // in minutes
    return (millis() - Last_discharge)/60000;
}

// ******** Getting the interval in minutes from potentiometer or the last discharge
int Get_interval_data() {
   int calc_int = (1+(Discharges[DISCHARGE_ARRAY-1]/10)); // calculated interval based on last discharge
   int tmpint = int(analogRead(INTERVAL_PIN)/3.0);
   if (tmpint > 0) {
      return tmpint;  // interval using the potentiometer
   } else if (calc_int > 0) { // if potentiometer=0, use calculated one
      return calc_int;
   } else {
      return 1;
   }
}
int Get_interval_web() {
   return max(Get_interval_data(), MIN_WEB_UPDATE_TIME);
}
//***************** Serial Event whenever incoming serial received
//void serialEvent(){
void readSerial() {
   String readstr = Serial.readStringUntil('>');
   while (readstr.length()>0) {  // in case any leftover older input
     readstr = Serial.readStringUntil('>');
   }
   readstr = Serial.readStringUntil('<'); //13=carriage return
   if (readstr.length()>0 && readstr.indexOf("D=") == 0) {
      int tmp_data = readstr.substring(readstr.indexOf("D=")+2).toInt();
      if (tmp_data < MAX_DISTANCE) {
        counting++;
        //count_web++;
        Newdata = tmp_data;
        Avgdata = (Avgdata*(counting-1) + Newdata)/counting;
        
        printlcd(toprowstr, readstr + "," + floatToString(Avgdata)+",C=" + String( max(int(1+(Get_interval_web()*60000-millis()+Last_web_update)/SCREEN_UPDATE),0)),1000);
      
        if(Newdata <= Dist_water_min) { // flooding is immiment
          tone(Buzzer_pin, 1600);
        }
      }
   } else if (readstr.length()>0 && readstr.indexOf("Comma=") == 0) {
      int tmp_pts = readstr.substring(readstr.indexOf("Comma=")+6).toInt() + 1;
      printlcd("SD pts:" + String(tmp_pts), "Expected:" + String(DATA_SIZE+DISCHARGE_ARRAY), 5000);
      if (tmp_pts == DATA_SIZE+DISCHARGE_ARRAY) {
         FTP_Web(); 
      }
   } else {
      printlcd("Unrecognized", readstr,1000);
   }
   while (Serial.available()) { // read the remaining incoming if any
      char ch = Serial.read();
   }
}

// ************************ Format for sending data to website ****************
//void send_to_Web() {
void update_SD() {
  printlcd("Update SD file", "Please wait", 100);
  //String jsData =  ">Web_header=var dist_interval=" + String(Get_interval_web()) + ";var dist=[<";
  String jsData =  ">Web_header=var dist=[<";
  Serial.print(jsData);
  Serial.flush();
  delay(1000);
  jsData = ">Data=" + floatToString(DataArr[0]);
  byte nn = 1; // above start the first one already so nn=1
  for (int n=1; n<DATA_SIZE-1; n++) {
    nn++;
    if (nn < 5) { // send multiple data each time
      jsData += "," + floatToString(DataArr[n]); 
    } else {
      Serial.print(jsData + "<");
      Serial.flush(); delay(1000);
      nn = 1;
      jsData = ">Data=," + floatToString(DataArr[n]); 
    }
  }
  jsData += "," + floatToString(DataArr[DATA_SIZE-1]) + "<";
  Serial.print(jsData);
  Serial.flush(); delay(1000);
  
  // Last discharge in minutes
  jsData = ">Data=];var disch=[<";
  Serial.print(jsData);
  Serial.flush(); delay(1000);
  
  jsData = ">Data=";
  for (int n=0; n<DISCHARGE_ARRAY; n++) {
    nn++;
    jsData += String(Discharges[n]) + ","; // in minutes
  }
  jsData += String(Get_lastdischarge()) + "];<"; // in minutes
  Serial.print(jsData);
  Serial.flush(); delay(1000);
  
  Serial.print(">SD_comma<");
  
  Last_web_update = millis();
}

void FTP_Web() {
  printlcd("FTP to Web site", "Please wait",100);
  Serial.print(">Web_update<");
  Serial.flush();
  printlcd("Update Website", "Completed", 5000);
  //Last_web_update = millis();
}

// *********************** Convert Float to String with one decimal point *****
String floatToString(float number) {
   int decint = int(number*10)%10; // the first decimal
   return String(int(number)) + "." + String(decint);
}

// ***************** Add to the depth array **************
void add_depth_array() { // the last index for ongoing discharge time
  printlcd(toprowstr, "Update:Avg=" + floatToString(Avgdata),1000);
  if (Avgdata > 0) {
     for(byte n=0; n < DATA_SIZE-1; n++) {
        DataArr[n] = DataArr[n+1]; // shifting array to the left
     }
     DataArr[DATA_SIZE-1] = Avgdata; // assign the new on the last index
     Avgdata = 0;    // reset to zero
     counting = 0;
  }
}

// ***************** Add to the discharge array **************
void add_discharge_array() {
  for(byte n=0; n < DISCHARGE_ARRAY-1; n++) { // last index for the latest/ongoing discharge time
     Discharges[n] = Discharges[n+1]; // shifting array to the left
  }
  Discharges[DISCHARGE_ARRAY-1] = Get_lastdischarge(); // assign the new on the 2nd last index
  Last_discharge = millis();  // refresh the last one
}

//************************** LCD output *********************
void printlcd(String strone, String strtwo, unsigned long delaytime) {
  int colonindex;
  noInterrupts();
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
  
  interrupts();
  if (delaytime>0) {
    delay(delaytime);
  }
}
