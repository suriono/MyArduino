void tft_init() {
    tft.begin();
  tft.setRotation(1);
  tft.fillScreen(0);
  tft.setTextSize(3);
  tft.setTextColor(tft.color565(0, 0 , 250));
  
  //tft.fillRect(100, 50, 20, 30, tft.color565(200, 200, 0)); // x,y, w, h, color
  
  tft.setCursor(0, 0);  tft.print("Basement");
  tft.setCursor(0, 30); tft.print("freezer");
}


void tft_message(String new_String) {
  static String last_String;
  //String new_String;
  uint16_t color_text;

  Serial.println(new_String);

  color_text = tft.color565(250, 0 , 0);

 // if (new_String != last_String) {
  
      tft.setCursor(150, 5);
      tft.setTextColor(ILI9341_BLACK);
      tft.print(last_String);
  
      tft.setCursor(150, 0);
      tft.setTextColor(color_text);    
      tft.print(new_String);
      
      last_String = new_String;
 // }
}
