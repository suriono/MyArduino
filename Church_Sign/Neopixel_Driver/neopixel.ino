String TextSign;

//uint16_t ColorText = Neopixel_Red;
//uint16_t Color2 = Neopixel_Red;
byte Colors[3] = {200,0,0};                       // = [200, 0, 0];
//byte Colors2[3] = {100,0,200};                      //= [150, 0, 200];
byte Width = 0;                     // widht of each letter
byte MinBrightness = 50;                // min brightness so it does not go dark at night
byte MaxBrightness = 230; 


//byte TextMode = 0;       // 0=normal text,1=pixel row 1,2=pixel row 2,3=pixel row 1,2

// ===================================================

void Neopixel_Initial(String initext, byte bright) {
  //delay(5000);   // so no big surge of current at the beginning
  matrix.begin();
  matrix.setTextWrap(false); 
  matrix.setBrightness(bright);  // from 0 to 255
  matrix.setTextColor(matrix.Color(200, 0, 0));
  matrix.setTextSize(1);
  matrix.fillScreen(0);  
 
  TextSign = initext;
  Width = 9;
  Neopixel_Display_Normal_Text();
}

// ==============================================

void Neopixel_Process_Input_Serial(String inputstr) {
  
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& jsonob = jsonBuffer.parseObject(inputstr);
  //DynamicJsonDocument jsonFromText;
  //deserializeJson(jsonFromText, inputstr);
  //JsonObject JsonObj = jsonFromText.as<JsonObject>();
  if (!jsonob.success()) {
    Serial.println("parseObject() failed");
    return;
  }

  isCursorEnable = false;
  
  Serial.print("Neopixel inputstr:"); Serial.println(inputstr);
  if (inputstr.indexOf("text") > 0) {
    Width = jsonob["width"];
    byte fontsize = jsonob["fontsize"];
    String tmpstr = jsonob["text"];   TextSign = tmpstr;
    matrix.setTextSize(fontsize);

    //matrix.fillScreen(0);
    Neopixel_Display_Normal_Text();
   
  } else if (inputstr.indexOf("color") > 0) {
    for (byte nn=0 ; nn<3 ; nn++) Colors[nn] = jsonob["color"][nn];

    matrix.setTextColor(Neopixel_Black);
    Neopixel_Display_Normal_Text();
    matrix.setTextColor(matrix.Color(Colors[0], Colors[1], Colors[2]));
    Neopixel_Display_Normal_Text();
    
  } else if (inputstr.indexOf("command") > 0) {
    String cmd = jsonob["command"];
    if (cmd.indexOf("clearall") > -1) matrix.fillScreen(0);
    matrix.show();
  //} else if (inputstr.indexOf("moveCursor") > 0) {
  //  moveCursor(jsonob["moveCursor"]);
  } else if (inputstr.indexOf("pix") > 0) {
    Serial.println(sizeof(jsonob["pix"]));
    int nn = 0;
    boolean isnotend = true;
    uint16_t pixnum;
    uint32_t pixcolor;
    while (nn < 80 && isnotend) {
      if (nn%2) {   // odd for color
        pixcolor = jsonob["pix"][nn];
        isnotend = (pixcolor > 0);
        if (isnotend) {
          matrix.setPixelColor(pixnum, pixcolor);
        }
        //Serial.println(pixcolor);
      } else {
        pixnum = jsonob["pix"][nn];
        //Serial.print(pixnum); Serial.print(",");
      }
      
      nn++;
    }
    matrix.show();
    /*
    for(uint16_t nn=0 ; nn<sizeof(jsonob["pix"])/2; nn++) {
      if (nn%2) {
        if (sizeof
        uint32_t pixcolor = jsonob["pix"][nn];
        Serial.println(pixcolor);
      }
    }*/
  } else if (inputstr.indexOf("cursor") > 0) {
    xcursor = jsonob["cursor"][0];
    ycursor = jsonob["cursor"][1];
  }
}

// ============================================

void Neopixel_Display_Normal_Text() {
  if (Width < 1) {         // normal text
    matrix.setCursor(xcursor, ycursor);
    matrix.print(TextSign);
  } else {
    for (byte nn=0; nn<TextSign.length() ; nn++) {
      matrix.setCursor(xcursor+nn*Width, ycursor);
      matrix.print(TextSign[nn]);
    }
  }
  matrix.show();
}

/*
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
*/
