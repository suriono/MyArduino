String Text1, Text2;                      // text for each row
uint16_t Color1 = Neopixel_Red;
uint16_t Color2 = Neopixel_Red;
byte Colors1[3] = {200,0,0};                       // = [200, 0, 0];
byte Colors2[3] = {100,0,200};                      //= [150, 0, 200];
int Width1 = 8; int Width2 = 8;                     // widht of each letter

// ===================================================

void Neopixel_Initial() {
  delay(5000);   // so no big surge of current at the beginning
  matrix1.begin(); matrix2.begin();
  matrix1.setTextWrap(false); matrix2.setTextWrap(false);
  matrix1.setBrightness(200);  // from 0 to 255
  matrix2.setBrightness(200);
  // matrix2.setTextSize(2);  // font size
  Text1 = "L  O  V  E";
  Text2 = "Never Fails";
  //Neopixel_Display_Normal_Text();
  Neopixel_Colorful_Text(Text1, Text2);
}

// ==============================================

void Neopixel_Process_Input_Serial(String inputstr) {
  
  deserializeJson(jsonFromText, inputstr);
  JsonObject JsonObj = jsonFromText.as<JsonObject>();

  Serial.print("Neopixel inputstr:"); Serial.println(inputstr);

  if (inputstr.indexOf("text1") > 0) {
    String row1 = JsonObj["text1"]; Text1 = row1;
    String row2 = JsonObj["text2"]; Text2 = row2;
    Width1 = max(JsonObj["width1"],6);
    Width2 = max(JsonObj["width2"],6);
    //String width1 = JsonObj["width1"]; Width1 = max(width1.toInt(),6);
    //String width2 = JsonObj["width2"]; Width2 = max(width2.toInt(),6);
    //if (Wid < 6) Spacing = 6;
    Serial.print("row1: "); Serial.println(Text1);
    Serial.print("row2: "); Serial.println(Text2);
    //Serial.print("spacing: "); Serial.println(Spacing);
    Neopixel_Display_Normal_Text();
  } else if (inputstr.indexOf("color") > 0) {
    if (inputstr.indexOf("color1") > 0) {
      for (byte nn=0 ; nn<3 ; nn++) Colors1[nn] = JsonObj["color1"][nn];
      Color1 = matrix1.Color(Colors1[0], Colors1[1], Colors1[2]);
    } else {
      for (byte nn=0 ; nn<3 ; nn++) Colors2[nn] = JsonObj["color2"][nn];
      Color2 = matrix1.Color(Colors2[0], Colors2[1], Colors2[2]);
    }
    Neopixel_Display_Normal_Text();
  } else if (inputstr.indexOf("brightness") > 0) {
    int brigh = max(int(JsonObj["brightness"]), 50); // minimum of 50
    Serial.print("Set Brightness: "); Serial.println(brigh);
    matrix1.setBrightness(brigh); matrix2.setBrightness(brigh);
    Neopixel_Display_Normal_Text();
  }
}

// ============================================

void Neopixel_Display_Normal_Text() {
  Neopixel_Set_Colors(Color1, Color2);
  matrix1.fillScreen(0);  matrix2.fillScreen(0);// erase everything
  matrix1.setCursor(0, 0); matrix2.setCursor(0, 0);
  for (byte nn=0; nn<Text1.length(); nn++) {
    matrix1.setCursor(nn*Width1, 0);
    matrix1.print(Text1.charAt(nn));
  }
  for (byte nn=0; nn<Text2.length(); nn++) {
    matrix2.setCursor(nn*Width2, 0);
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
  matrix1.Color(155, 0, 0), matrix1.Color(0, 155, 0), matrix1.Color(155, 155, 0),matrix1.Color(0, 0, 155), matrix1.Color(155, 0, 155), matrix1.Color(0, 155, 155), matrix1.Color(155, 155, 155)};
  matrix1.fillScreen(0);  matrix2.fillScreen(0);// blank the screen
  byte color_num = 0;
  for (byte nn=0; nn<inputstr1.length(); nn++) {
    if (color_num > 6) color_num = 0;
    matrix1.setTextColor(colors[color_num]);
    matrix1.setCursor(nn*8, 0);
    matrix1.print(Text1.charAt(nn));
    color_num++;
  }
  matrix1.show();
  color_num = 0;
  for (byte nn=0; nn<inputstr2.length(); nn++) {
    if (color_num > 6) color_num = 0;
    matrix2.setTextColor(colors[color_num]);
    matrix2.setCursor(nn*8, 0);
    matrix2.print(Text2.charAt(nn));
    color_num++;
  }
  matrix2.show();
}


// ================== Adjust brightness ==================

void Neopixel_Adjust_Brightness() {
    static boolean toggle_brightness;
    int brightness = map(analogRead(A0), 0, 1023, 255, 40);
    Serial.print("Light sensor (40-255) : "); Serial.println(brightness);

    byte tmpcol1[3];
    for (byte nn=0; nn<3 ; nn++) {
      tmpcol1[nn] = Colors1[nn] * brightness / 255;
    }
    Color1 = matrix1.Color(tmpcol1[0], tmpcol1[1], tmpcol1[2]);
    for (byte nn=0; nn<3 ; nn++) {
      tmpcol1[nn] = Colors2[nn] * brightness / 255;
    }
    Color2 = matrix2.Color(tmpcol1[0], tmpcol1[1], tmpcol1[2]);
    Neopixel_Display_Normal_Text();
    
}


