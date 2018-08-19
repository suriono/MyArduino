// ===================================================

void Neopixel_Initial() {
  matrix1.begin(); matrix2.begin();
  matrix1.setTextWrap(false); matrix2.setTextWrap(false);
  matrix1.setBrightness(60);  // from 0 to 255
  matrix2.setBrightness(128);
  
  Neopixel_Colorful_Text("All welcome to", "River Hills");
}

// ===============================================

void Neopixel_Colorful_Text(String inputstr1, String inputstr2) {
  
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

