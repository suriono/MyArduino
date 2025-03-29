
//=============================================

void tft_init() {
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(0);
  tft.setTextSize(3);
  
  //tft.fillRect(100, 50, 20, 30, tft.color565(200, 200, 0)); // x,y, w, h, color
  tft.setTextColor(tft.color565(0, 0 , 250));
  tft.setCursor(0,0);  tft.print("Basement");
  tft.setCursor(0,36);  tft.print("Freezer");
  tft.drawLine(0,75, 320, 75, tft.color565(250,111,3));

  tft.setTextColor(tft.color565(250, 120 , 0));
  tft.setCursor(0,90);  tft.print("Top");
  tft.setCursor(0,126);  tft.print("Fridge");
  tft.drawLine(0,165, 320, 165, tft.color565(250,111,3));

  tft.setTextColor(tft.color565(200, 50 , 220));
  tft.setCursor(0,180);  tft.print("Bottom");
  tft.setCursor(0,216);  tft.print("Fridge");
  //tft.setTextColor(tft.color565(200, 200 , 0));
  //tft.setCursor(0, 125); tft.print("Countdown");
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

void tft_message(int door_num, String upstr, String downstr, uint16_t color_text) {
   static String last_uppestr[3];
   static String last_downstr[3];
   tft.setTextSize(4); // fontsize);

   if ( upstr.length() > 0 ) {
      tft.setCursor(160, door_num*90);
      tft.setTextColor(ILI9341_BLACK);
      tft.print(last_uppestr[door_num]);
  
      tft.setCursor(160, door_num*90);
      tft.setTextColor(color_text);    
      tft.print(upstr);
      last_uppestr[door_num] = upstr;
   }

   if ( downstr.length() > 0 ) {
      tft.setCursor(160, door_num*90+33);
      tft.setTextColor(ILI9341_BLACK);
      tft.print(last_downstr[door_num]);
  
      tft.setCursor(160, door_num*90+33);
      tft.setTextColor(color_text);    
      tft.print(downstr);
      last_downstr[door_num] = downstr;
   } else if ( last_downstr[door_num].length() > 0) {
      Serial.println("delete bottom");
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
