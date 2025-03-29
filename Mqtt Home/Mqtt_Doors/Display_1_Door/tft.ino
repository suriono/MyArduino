void tft_init() {
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(0);
  tft.setTextSize(3);
  tft.setTextColor(tft.color565(0, 0 , 250));
  
  //tft.fillRect(100, 50, 20, 30, tft.color565(200, 200, 0)); // x,y, w, h, color
  
  tft.setCursor(18, 0);  tft.print("Basement freezer");
  tft.drawLine(0,110, 320, 110, tft.color565(250,111,3));

  tft.setTextColor(tft.color565(150, 200 , 0));
  tft.setCursor(0, 125); tft.print("Countdown");
}

// ===========================================

void tft_basement_freezer(String msg) {
  static String last_String;
  String new_String;
  uint16_t color_text;
  myJSON = JSON.parse(msg);
  if (JSON.typeof(myJSON) != "undefined") {
    if ( myJSON.hasOwnProperty("countDown_Alarm")) {
      countDown_Alarm = (int) myJSON["countDown_Alarm"];
      if (countDown_Alarm > 0) {  // when the door is open
         new_String = "OPEN";
         color_text = tft.color565(250, 0 , 0);
      } else {                    // door close, countDown_Alarm=0
         tft_erase_message("Close the    freezer !!!", 0, 160, 4);
         new_String = "CLOSE";  
         tft_countDown();
         color_text = tft.color565(0, 250 , 0);
      }

      tft_message(last_String, new_String, color_text, 60, 35, 8);
      last_String = new_String;

      tft_screen_OnOff(true);
      countDown_screenOFF = 240;
    }
  }
}

// ===========================================

void tft_message(String last_String, String new_String, uint16_t color_text, int X, int Y, byte fontsize) {
      tft.setTextSize(fontsize);
      tft.setCursor(X, Y);
      tft.setTextColor(ILI9341_BLACK);
      tft.print(last_String);
  
      tft.setCursor(X, Y);
      tft.setTextColor(color_text);    
      tft.print(new_String);
}

// ============================================

void tft_erase_message(String msgstr, int X, int Y, byte fontsize) {
  tft.setTextSize(fontsize);
  tft.setCursor(X, Y);
  tft.setTextColor(ILI9341_BLACK);
  tft.print(msgstr);
}

// ============================================

void tft_countDown() {
  static String last_String;
  String new_String;
  uint16_t color_text;
  static boolean isFirst_Alarm = false;

  if (countDown_Alarm == 1) {
    if ( !last_String.equals("Close the    freezer !!!")) tft_erase_message(last_String, 140, 160, 10);
    new_String = "Close the    freezer !!!";
    color_text = tft.color565(250, 0 , 250); 
    tft_message(last_String, new_String, color_text, 0, 160, 4);
    if (isFirst_Alarm) {
      isFirst_Alarm = false;
      smtp_send();  // Email just before beeping
      //smtp_send();  // Email just before beepin
    }
  } else {
    new_String = String(countDown_Alarm);
    color_text = tft.color565(168, 50 , 121); 
    tft_message(last_String, new_String, color_text, 140, 160, 10);
    
    if (countDown_Alarm == 2) {
      isFirst_Alarm = true;
    }
  }
  last_String = new_String;
}

// ============================================

void tft_screen_OnOff(bool isON) {
   digitalWrite(TFT_LED, isON);
}
