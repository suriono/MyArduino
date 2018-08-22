String Text1, Text2; // text for each row

// ===================================================

void Neopixel_Initial() {
  matrix1.begin(); matrix2.begin();
  matrix1.setTextWrap(false); matrix2.setTextWrap(false);
  matrix1.setBrightness(60);  // from 0 to 255
  matrix2.setBrightness(128);
  
  Neopixel_Colorful_Text("All welcome to", "River Hills");
}

// ==============================================

void Neopixel_Process_Input_Serial(String inputstr) {
  Neopixel_Colorful_Text(inputstr,"River Hills");
  deserializeJson(jsonFromText, inputstr);
  JsonObject JsonObj = jsonFromText.as<JsonObject>();
  String row1 = JsonObj["text1"];
  Serial.print("row1: "); Serial.println(row1);
  //char json[] = inputstr;
}

// ============================================

void Neopixel_Display_Normal_Text(String inputstr1, String inputstr2) {
  matrix1.fillScreen(0);  matrix2.fillScreen(0);// erase everything
  Neopixel_Set_Colors(Neopixel_Red, Neopixel_Red) ; // both red
  matrix1.setCursor(0, 0); matrix2.setCursor(0, 0);
  matrix1.print(inputstr1); matrix2.print(inputstr2);
  matrix1.show(); matrix2.show();
}

// =============================================

void Neopixel_Set_Colors(uint16_t color1, uint16_t color2) {
  matrix1.setTextColor(color1);
  matrix2.setTextColor(color2);
}

// ===============================================

void Neopixel_Colorful_Text(String inputstr1, String inputstr2) {
  uint16_t colors[] = {
  matrix1.Color(255, 0, 0), matrix1.Color(0, 255, 0), matrix1.Color(255, 255, 0),matrix1.Color(0, 0, 255), matrix1.Color(255, 0, 255), matrix1.Color(0, 255, 255), matrix1.Color(255, 255, 255)};
  
  matrix1.fillScreen(0);  matrix2.fillScreen(0);// blank the screen
  
  byte color_num = 0;
  for (byte nn=0; nn<inputstr1.length(); nn++) {
    if (color_num > 6) color_num = 0;
    matrix1.setTextColor(colors[color_num]);
    matrix1.setCursor(nn*6, 0);
    matrix1.print(inputstr1.charAt(nn));
    color_num++;
  }
  matrix1.show();

  color_num = 0;
  for (byte nn=0; nn<inputstr2.length(); nn++) {
    if (color_num > 6) color_num = 0;
    matrix2.setTextColor(colors[color_num]);
    matrix2.setCursor(nn*6, 0);
    matrix2.print(inputstr2.charAt(nn));
    color_num++;
  }
  matrix2.show();
}

