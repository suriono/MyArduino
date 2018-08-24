String Text1, Text2; // text for each row
uint16_t Color1 = Neopixel_Red;
uint16_t Color2 = Neopixel_Red;
int Spacing = 8;

// ===================================================

void Neopixel_Initial() {
  matrix1.begin(); matrix2.begin();
  matrix1.setTextWrap(false); matrix2.setTextWrap(false);
  matrix1.setBrightness(60);  // from 0 to 255
  matrix2.setBrightness(60);
  // matrix2.setTextSize(2);  // font size
  Text1 = "Welcome to";
  Text2 = "River Hills";
  //Neopixel_Display_Normal_Text();
  Neopixel_Colorful_Text("Love Never Fails", "River Hills");
}

// ==============================================

void Neopixel_Process_Input_Serial(String inputstr) {
  
  deserializeJson(jsonFromText, inputstr);
  JsonObject JsonObj = jsonFromText.as<JsonObject>();

  Serial.print("Neopixel inputstr:"); Serial.println(inputstr);

  if (inputstr.indexOf("text1") > 0) {
    String row1 = JsonObj["text1"]; Text1 = row1;
    String row2 = JsonObj["text2"]; Text2 = row2;
    String spac = JsonObj["spacing"]; Spacing = spac.toInt();
    if (Spacing < 6) Spacing = 6;
    Serial.print("row1: "); Serial.println(Text1);
    Serial.print("row2: "); Serial.println(Text2);
    Serial.print("spacing: "); Serial.println(Spacing);
    Neopixel_Display_Normal_Text();
  } else if (inputstr.indexOf("color") > 0) {
    byte color[3];

    if (inputstr.indexOf("color1") > 0) {
      for (byte nn=0 ; nn<3 ; nn++) color[nn] = JsonObj["color1"][nn];
      Color1 = matrix1.Color(color[0], color[1], color[2]);
    } else {
      for (byte nn=0 ; nn<3 ; nn++) color[nn] = JsonObj["color2"][nn];
      Color2 = matrix1.Color(color[0], color[1], color[2]);
    }
    Serial.print("Color: "); Serial.print(color[0]); Serial.print(",");
    Serial.print(color[1]); Serial.print(","); Serial.println(color[2]); 
    
    //uint16_t row2_color = matrix2.Color(color2[0], color2[1], color2[2]);
    //Neopixel_Set_Colors(row1_color, row2_color);
    Neopixel_Display_Normal_Text();
  }
}

// ============================================

void Neopixel_Display_Normal_Text() {
  Neopixel_Set_Colors(Color1, Color2);
  matrix1.fillScreen(0);  matrix2.fillScreen(0);// erase everything
  matrix1.setCursor(0, 0); matrix2.setCursor(0, 0);
  for (byte nn=0; nn<Text1.length(); nn++) {
    matrix1.setCursor(nn*Spacing, 0);
    matrix1.print(Text1.charAt(nn));
  }
  for (byte nn=0; nn<Text2.length(); nn++) {
    matrix2.setCursor(nn*Spacing, 0);
    matrix2.print(Text2.charAt(nn));
  }
  matrix1.show(); matrix2.show();
}

// =============================================

void Neopixel_Set_Colors(uint16_t color1, uint16_t color2) {
  matrix1.setTextColor(color1);
  matrix2.setTextColor(color2);
}

// ===============================================

void Neopixel_Colorful_Text(String inputstr1, String inputstr2) {
  Text1 = inputstr1; Text2 = inputstr2;
  uint16_t colors[] = {
  matrix1.Color(255, 0, 0), matrix1.Color(0, 255, 0), matrix1.Color(255, 255, 0),matrix1.Color(0, 0, 255), matrix1.Color(255, 0, 255), matrix1.Color(0, 255, 255), matrix1.Color(255, 255, 255)};
  
  matrix1.fillScreen(0);  matrix2.fillScreen(0);// blank the screen
  
  byte color_num = 0;
  for (byte nn=0; nn<inputstr1.length(); nn++) {
    if (color_num > 6) color_num = 0;
    matrix1.setTextColor(colors[color_num]);
    matrix1.setCursor(nn*6, 0);
    matrix1.print(Text1.charAt(nn));
    color_num++;
  }
  matrix1.show();

  color_num = 0;
  for (byte nn=0; nn<inputstr2.length(); nn++) {
    if (color_num > 6) color_num = 0;
    matrix2.setTextColor(colors[color_num]);
    matrix2.setCursor(nn*6, 0);
    matrix2.print(Text2.charAt(nn));
    color_num++;
  }
  matrix2.show();
}


