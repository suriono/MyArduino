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
  } else if (Cmd.equals("SetBrightness")) {
     Set_Brightness();
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

// ============== Set Brightness ===============
void Set_Brightness() {
  matrix.setBrightness(Json_parse_int("Bright"));
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
  for (int nn=0; nn < 80 ; nn++) {
    SerialUSB.println(nn);
    SerialUSB.println(matrix.getPixelColor(nn));
  }
}
