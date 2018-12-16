String TextSign;

//uint16_t ColorText = Neopixel_Red;
//uint16_t Color2 = Neopixel_Red;
byte Colors[3] = {200,0,0};                       // = [200, 0, 0];
//byte Colors2[3] = {100,0,200};                      //= [150, 0, 200];
byte Width = 0;                     // widht of each letter
byte MinBrightness = 100;                // min brightness so it does not go dark at night
byte MaxBrightness = 220; 


//byte TextMode = 0;       // 0=normal text,1=pixel row 1,2=pixel row 2,3=pixel row 1,2



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
  } else if (inputstr.indexOf("cursor") > 0) {
    xcursor = jsonob["cursor"][0];
    ycursor = jsonob["cursor"][1];
  } else if (inputstr.indexOf("Bright") > 0) {
    if (inputstr.indexOf("min") > 0) MinBrightness = jsonob["minBright"];
    
    if (inputstr.indexOf("max") > 0) MaxBrightness = jsonob["maxBright"];
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


// ================== Adjust brightness ==================

void Neopixel_Adjust_Brightness() {
    //Serial.print("max"); Serial.println(MaxBrightness);
    int brightness_sensor = map(analogRead(A0), 0, 1023, MaxBrightness, MinBrightness);
    //Serial.print("Light sensor (0-255) : "); Serial.println(brightness_sensor);

    matrix.setBrightness(brightness_sensor);
    matrix.show();
}

// ===================================================

void Neopixel_Initial() {
  //delay(5000);   // so no big surge of current at the beginning
  matrix.begin();
  matrix.setTextWrap(false); 
  matrix.setBrightness(200);  // from 0 to 255
  
  matrix.setTextSize(2);
  matrix.fillScreen(0); 
/*
  matrix.setTextSize(1);
  matrix.setBrightness(20);  // from 0 to 255
  matrix.setTextColor(matrix.Color(0, 255, 0));
  xcursor = 0; ycursor = 0;
  TextSign = "River";
  Neopixel_Display_Normal_Text();

*/
  matrix.setTextColor(matrix.Color(0, 255, 0));
  ycursor = 13;
  TextSign = "OPEN";
  Neopixel_Display_Normal_Text();

  matrix.setTextColor(matrix.Color(255, 0, 0));
  xcursor =50; ycursor = 0;
  TextSign = "Heart";
  Neopixel_Display_Normal_Text();

  matrix.setTextColor(matrix.Color(29, 219, 159));
  xcursor =85; ycursor = 16;
  TextSign = "Minds";
  Neopixel_Display_Normal_Text();

  matrix.setTextColor(matrix.Color(237, 23, 100));
  xcursor =146; ycursor = 8;
  TextSign = "Door";
  Neopixel_Display_Normal_Text();

  matrix.setTextSize(1);
  matrix.setTextColor(matrix.Color(255, 0, 0));
  xcursor =0; ycursor = 32;
  TextSign = "www.RiverHillsUMC.org";
  Neopixel_Display_Normal_Text(); 
}
