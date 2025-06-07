
//=============================================

void tft_init() {
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(0);
  tft.setTextSize(2);
  
  //tft.fillRect(100, 50, 20, 30, tft.color565(200, 200, 0)); // x,y, w, h, color
  tft.setTextColor(tft.color565(0, 0 , 250));
  tft.setCursor(0,0);  tft.print("Freezer");
  //tft.setCursor(0,20);  tft.print("Freezer");
  tft.drawLine(0,75, 320, 75, tft.color565(250,111,3));

  tft.setTextColor(tft.color565(250, 120 , 0));
  tft.setCursor(0,80);  tft.print("Top Fridge");
  //tft.setCursor(0,100);  tft.print("Fridge");
  tft.drawLine(0,155, 320, 165, tft.color565(250,111,3));

  tft.setTextColor(tft.color565(200, 50 , 220));
  tft.setCursor(0,160);  tft.print("Bottom Fridge");
  //tft.setCursor(0,180);  tft.print("Fridge");

  tft.drawLine(160,0, 160, 240, tft.color565(250,111,3));  // vertical middle line

  tft.setTextColor(tft.color565(0, 100 , 150));
  tft.setCursor(170,0);  tft.print("Current Flow");
  tft.setCursor(170,20);  tft.print("(gal/min)");

  tft.setTextColor(tft.color565(120, 10 , 150));
  tft.setCursor(170,80);  tft.print("Today Usage");
  tft.setCursor(170,100);  tft.print("(Gallons)");

  tft.setTextColor(tft.color565(120, 100 , 50));
  tft.setCursor(170,165);  tft.print("Continuous");
  tft.setCursor(170,185);  tft.print("Flow (Hours)");
}

// ===========================================

void tft_basement_freezer(String msg) {
  static String last_String;
  String new_String, count_str;
  uint16_t color_text;
  myJSON = JSON.parse(msg);
  if (JSON.typeof(myJSON) != "undefined") {
    if ( myJSON.hasOwnProperty("countDown_Alarm")) {
      countDown_Alarm = (int) myJSON["countDown_Alarm"];
      if (countDown_Alarm > 0) {  // when the door is open
         new_String = "OPEN"; count_str = "";
         color_text = tft.color565(250, 0 , 0);
      } else {                    // door close, countDown_Alarm=0
         //tft_erase_message("Close the    freezer !!!", 0, 160, 4);
         new_String = "CLOSE"; count_str = "";
         tft_countDown();
         color_text = tft.color565(0, 250 , 0);
      }

      tft_message(Door_Number, new_String, count_str, color_text);
      last_String = new_String;

      tft_screen_OnOff(true);
      countDown_screenOFF = TFT_SCREEN_OFF_COUNTDOWN;
    }
  }
}

// ===========================================

void tft_water(String msg) {
  static String last_Flow, last_Today, last_Continous;
  String new_String, count_str;
  uint16_t color_text;
  
  myJSON = JSON.parse(msg);
  if (JSON.typeof(myJSON) != "undefined") {
    tft.setTextSize(3); // fontsize);
    Serial.print("Water Flow now: "); Serial.println(msg);
    if ( myJSON.hasOwnProperty("current_water_flow")) {
      double tmp = (double) myJSON["current_water_flow"];
      new_String = String(tmp);  
      tft.setCursor(170, 43); tft.setTextColor(ILI9341_BLACK); tft.print(last_Flow); // erase first
      tft.setCursor(170, 43); tft.setTextColor(tft.color565(0, 250 , 0)); tft.print(new_String);
      last_Flow = new_String;
    }
    if ( myJSON.hasOwnProperty("gallons_used_today")) {
      int tmp = (int) myJSON["gallons_used_today"];
      new_String = String(tmp);  
      tft.setCursor(170, 130); tft.setTextColor(ILI9341_BLACK); tft.print(last_Today); // erase first
      tft.setCursor(170, 130); tft.setTextColor(tft.color565(0, 250 , 0)); tft.print(new_String);
      last_Today = new_String;
    }
    if ( myJSON.hasOwnProperty("hours_continuous_flow")) {
      int tmp = (int) myJSON["hours_continuous_flow"];
      new_String = String(tmp);
      tft.setCursor(170, 210); tft.setTextColor(ILI9341_BLACK); tft.print(last_Continous); // erase first
      tft.setCursor(170, 210); tft.setTextColor(tft.color565(0, 250 , 0)); tft.print(new_String);
      last_Continous = new_String;
    }
  }
}

// ===========================================

void tft_message(int door_num, String upstr, String downstr, uint16_t color_text) {
   static String last_uppestr[3];
   static String last_downstr[3];
   tft.setTextSize(3); // fontsize);

   if ( upstr.length() > 0 ) {   // For OPEN or CLOSE
      tft.setCursor(0, door_num*80+50);
      tft.setTextColor(ILI9341_BLACK);  // erase first
      tft.print(last_uppestr[door_num]);
  
      tft.setCursor(0, door_num*80+50);
      tft.setTextColor(color_text);    
      tft.print(upstr);
      last_uppestr[door_num] = upstr;
   }

   if ( downstr.length() > 0 ) {
      tft.setCursor(100, door_num*80+50);
      tft.setTextColor(ILI9341_BLACK);  // erase the last counter first
      tft.print(last_downstr[door_num]);
  
      tft.setCursor(100, door_num*80+50);
      tft.setTextColor(color_text);    
      tft.print("100"); // tft.print(downstr);
      last_downstr[door_num] = downstr;
   } else if ( last_downstr[door_num].length() > 0) { // erase the last counter if any
      tft.setCursor(160, door_num*90+33);
      tft.setTextColor(ILI9341_BLACK);
      tft.print(last_downstr[door_num]);
      last_downstr[door_num] = "";
   }
}

// ============================================

void tft_countDown() {
  static String last_String;
  String new_String;
  uint16_t color_text;
  static boolean isFirst_Alarm = false;

  if (countDown_Alarm == 1) {
    //if ( !last_String.equals("Close the")) tft_erase_message(last_String, 140, 160, 10);
    //new_String = "Close the";
    color_text = tft.color565(250, 0 , 250); 
    tft_message(Door_Number, "Fridge", "open!!", color_text);
    if (isFirst_Alarm) {
      isFirst_Alarm = false;
      smtp_send();  // Email just before beeping
      //smtp_send();  // Email just before beepin
    }
  } else {
    if (countDown_Alarm != 0) {
       new_String = String(countDown_Alarm);
       color_text = tft.color565(168, 50 , 121); 
       tft_message(Door_Number, "", new_String, color_text);
    }
    
    if (countDown_Alarm == 2) {
      isFirst_Alarm = true;
    }
  }
//  last_String = new_String;
}

// ============================================

void tft_screen_OnOff(bool isON) {
   digitalWrite(TFT_LED, isON);
}
