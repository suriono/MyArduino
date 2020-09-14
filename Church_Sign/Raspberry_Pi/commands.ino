void Command_Run() {
  String Cmd = JSON.stringify(jsonObject["cmd"]); Cmd.replace("\"","");
  Serial.print(Cmd);
  Serial.print("========");
  Serial.println();
  if (Cmd.equals("DeleteAll")) {
     Delete_All();
  } else if (Cmd.equals("TestConnection")) {
     Serial.print("test connection OK");
     SerialUSB.println("OK");
  } else if (Cmd.equals("SetCursor")) {
     Set_Cursor();
  } else if (Cmd.equals("SetText")) {
     Set_Text();
  } else if (Cmd.equals("Test_NightBrightness")) {
     Test_Dimmer(70);
  } else if (Cmd.equals("SetPixels")) {
     Set_Pixels();
  } else if (Cmd.equals("SavePixel")) {
     Get_Pixels();
  }
}

// ============== Delete All ===============
void Delete_All() {
  matrix.fillScreen(0);
  matrix.show();
}

// ============== Test Dimmer ===============
void Test_Dimmer(int bright) {
  uint32_t pixcol[256*TILE_COLUMNS*TILE_COLUMNS], pixcolor,pixnight;
  uint32_t r, g, b;
  for (int nn=0; nn < 256*TILE_COLUMNS*TILE_COLUMNS ; nn++) {
    pixcol[nn] = matrix.getPixelColor(nn);
    
    if (pixcol[nn] > 0) {
      b = (pixcol[nn] & 255  ) * bright / 255;
      g = int(((pixcol[nn]>>8)&255) * bright / 255)<<8;
      r = int((pixcol[nn]>>16) * bright / 255)<<16;
      matrix.setPixelColor(nn, b+g+r);
    }
  }
  matrix.show();
  delay(3000);
  for (int nn=0; nn < 256*TILE_COLUMNS*TILE_COLUMNS ; nn++) {
    if (pixcol[nn] > 0) {
      matrix.setPixelColor(nn, pixcol[nn]);
    }
  }
  matrix.show();
}

// ============== Set Cursor ===============
void Set_Cursor() { 
  int x = Json_parse_int("X");
  int y = Json_parse_int("Y");
  matrix.setCursor(x,y);
}

// ============== Set Colors ===============
void Set_TextColor() { 
  int r = Json_parse_int("Red");
  int g = Json_parse_int("Green");
  int b = Json_parse_int("Blue");
  matrix.setTextColor(matrix.Color(r, g, b));
}

// ============== Set Text ===============
void Set_Text() {
  Set_Cursor();
  Set_TextColor();
  matrix.setTextSize(Json_parse_int("Size"));
  matrix.print(Json_parse_str("Text"));
  matrix.show();
}

// ============== Set Pixels ===============
void Set_Pixels() {
  JSONVar pp = JSON.parse(jsonObject["Pixels"]); // pair of pixel# & color
  Serial.println(pp);
  for (int nn=0; nn < pp.length()/2 ; nn++) {
    //Serial.println(int(pp[nn*2+1]));
    matrix.setPixelColor(int(pp[nn*2]), int(pp[nn*2+1]));
  }
  matrix.show();
}

// ============== Get Pixels ===============
void Get_Pixels() {
  uint32_t pixcolor;
  for (int nn=0; nn < 256*TILE_COLUMNS*TILE_COLUMNS ; nn++) {
    pixcolor = matrix.getPixelColor(nn);
    Serial.println(pixcolor);
    if (pixcolor > 0) {
       SerialUSB.println(nn);
       SerialUSB.println(pixcolor);
    }
  }
}
