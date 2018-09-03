String Text1, Text2;                      // text for each row
uint16_t Color1 = Neopixel_Red;
uint16_t Color2 = Neopixel_Red;
byte Colors1[3] = {200,0,0};                       // = [200, 0, 0];
byte Colors2[3] = {100,0,200};                      //= [150, 0, 200];
int Width1 = 8; int Width2 = 8;                     // widht of each letter
byte MinBrightness = 50;                // min brightness so it does not go dark at night
byte MaxBrightness = 230; 
byte TextMode = 0;       // 0=normal text,1=pixel row 1,2=pixel row 2,3=pixel row 1,2

// ===================================================

void Neopixel_Initial() {
  delay(5000);   // so no big surge of current at the beginning
  matrix1.begin(); matrix2.begin();
  matrix1.setTextWrap(false); matrix2.setTextWrap(false);
  matrix1.setBrightness(MaxBrightness);  // from 0 to 255
  matrix2.setBrightness(MaxBrightness);
  // matrix2.setTextSize(2);  // font size
  Text1 = "L  O  V  E";
  Text2 = "Never Fails";
  //Neopixel_Display_Normal_Text();
  Neopixel_Colorful_Text(Text1, Text2);
}

// ==============================================

void Neopixel_Process_Input_Serial(String inputstr) {
  DynamicJsonDocument jsonFromText;
  deserializeJson(jsonFromText, inputstr);
  JsonObject JsonObj = jsonFromText.as<JsonObject>();

  Serial.print("Neopixel inputstr:"); Serial.println(inputstr);

  if (inputstr.indexOf("text1") > 0) {
    TextMode = 0;
    String row1 = JsonObj["text1"]; Text1 = row1;
    String row2 = JsonObj["text2"]; Text2 = row2;
    Width1 = max(JsonObj["width1"],6);
    Width2 = max(JsonObj["width2"],6);
    Serial.print("row1: "); Serial.println(Text1);
    Serial.print("row2: "); Serial.println(Text2);
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
    MaxBrightness = max(int(JsonObj["brightness"]), 50); // minimum of 50
    Serial.print("Set Brightness: "); Serial.println(MaxBrightness);
    matrix1.setBrightness(MaxBrightness); matrix2.setBrightness(MaxBrightness);
    matrix1.show(); matrix2.show();
  } else if (inputstr.indexOf("minBright") > 0) {
    MinBrightness = max(int(JsonObj["minBright"]), 50); // minimum of 50
    Serial.print("MIN Brightness: "); Serial.println(MinBrightness);
  } else if (inputstr.indexOf("pixel") > 0) {  // pixel file for row 1
    if (JsonObj["pixel"] == 1) {
      TextMode = 1;
      matrix1.setCursor(0,0);
      matrix1.fillScreen(0);
      matrix1.show();
    } else {
      TextMode = 2;
      matrix2.setCursor(0,0);
      matrix2.fillScreen(0);
      matrix2.show();
    }
  } else if (inputstr.indexOf("X") > 0) {  // pixel file for row 1 & 2
    
      uint16_t R,G,B, x, y;
      R = JsonObj["R"];
      G = JsonObj["G"];
      B = JsonObj["B"];
      x = JsonObj["X"]; y = JsonObj["Y"];
      if (TextMode == 1) {
        matrix1.drawPixel(x,y,matrix1.Color(R,G,B));
        matrix1.show(); //delay(10);
      } else {
        Serial.print("==== pixel: "); Serial.print(x);Serial.print(",");Serial.print(y); Serial.println(inputstr);
        matrix2.drawPixel(x,y,matrix2.Color(R,G,B));
        matrix2.show(); //delay(10);
      }
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
    
    int brightness_sensor = map(analogRead(A0), 0, 1023, 255, MinBrightness);
    Serial.print("Light sensor (40-255) : "); Serial.println(brightness_sensor);

    // get minimum total
    int sum1 = 0;
    int sum2 = 0;
    for (byte nn=0; nn<3 ; nn++) {
      sum1 += Colors1[nn]; // * brightness_sensor / 255 / 3;
      sum2 += Colors2[nn]; // * brightness_sensor / 255 / 3;
    }

    int tmpcol1[3], tmpcol2[3];
    
    for (byte nn=0; nn<3 ; nn++) {
      tmpcol1[nn] = map(Colors1[nn],0,255, 0, brightness_sensor);
      tmpcol2[nn] = map(Colors2[nn],0,255, 0, brightness_sensor);
      //Serial.print(tmpcol1[nn]); Serial.print(","); Serial.println(tmpcol2[nn]);
    }

    
    /*
    int min_sum12 = min(sum1,sum2);// * MaxBrightness / 255;
    //Serial.print("Min sum brightness: "); Serial.println(String(min_sum12));

    if (min_sum12 < MinBrightness*3) { // below the minimum
      for (byte nn=0; nn<3 ; nn++) {
        tmpcol1[nn] = map(tmpcol1[nn],0,sum1, 0, MinBrightness);
        tmpcol2[nn] = map(tmpcol2[nn],0,sum2, 0, MinBrightness);
        //Serial.print(tmpcol1[nn]); Serial.print(","); Serial.println(tmpcol2[nn]);
      }
    }
    */
    Color1 = matrix1.Color(tmpcol1[0], tmpcol1[1], tmpcol1[2]);
    Color2 = matrix2.Color(tmpcol2[0], tmpcol2[1], tmpcol2[2]);

    if (TextMode == 0) {
      matrix1.setBrightness(MaxBrightness);  // from 0 to 255
      matrix2.setBrightness(MaxBrightness);
      Neopixel_Display_Normal_Text();
    } else {
      byte pixel_bright = map(brightness_sensor, 0, 255, 0, MaxBrightness);
      matrix1.setBrightness(pixel_bright);  // from 0 to 255
      matrix2.setBrightness(pixel_bright);
      matrix1.show(); matrix2.show();
      Serial.print("Pixel brightness: "); Serial.println(pixel_bright);
    }
    
}


