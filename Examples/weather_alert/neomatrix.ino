// ===============================================
void Neopixel_Initial() {
  matrix.begin(); 
  matrix.setTextWrap(true); 
  matrix.setBrightness(200);  // from 0 to 255
  matrix.setTextColor(matrix.Color(155, 0, 0));
  matrix.fillScreen(0);
  matrix.setCursor(0, 0);
  matrix.print(F("How"));
  matrix.show(); 
}

// ===================================================

void Neomatrix_scrolltext(String instr, byte R, byte G, byte B) {

  matrix.setTextColor(matrix.Color(R,G,B));

  //Serial.println(instr.length());
  for (int x=0 ; x < (instr.length()*CHAR_WIDTH+NCOLUMNS); x++) {
    matrix.fillScreen(0);
    matrix.setCursor(-x+NCOLUMNS-1, 0);

    matrix.print(instr);
    matrix.show();
    delay(60);
  }
}
