const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0),matrix.Color(0, 0, 255), matrix.Color(255, 0, 255), matrix.Color(0, 255, 255), matrix.Color(255, 255, 0), matrix.Color(255, 255, 255)};


void scroll_text() {
  static int x = matrix.width();
  static int color_rotation = 0;
  matrix.fillScreen(0);
  matrix.setTextColor(colors[color_rotation]);
  matrix.setCursor(x, 0);
  matrix.print(Incoming_Data);

  int total_length = Incoming_Data.length() * -6;
  //Serial.println(total_length);
  //Serial.println(x);

  if(--x < total_length) {
    x = matrix.width();
    if(++color_rotation >=6) color_rotation = 0;
  }
  matrix.show();
  
}
