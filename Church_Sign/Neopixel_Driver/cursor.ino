byte xcursor, ycursor;

void moveCursor(uint16_t newpos) {
  Serial.println(newpos);
  matrix.setPixelColor(cursorNum, cursorColor_last);
  cursorNum = newpos;
  cursorColor_last = matrix.getPixelColor(newpos);
  isCursorEnable = true;
}

void getCursorNum() {
  cursorNum = xcursor * 8 + (xcursor % 2)*7 + (((xcursor%2)<1)-((xcursor%2)>0))*(ycursor%8 + (ycursor/8)*TILE_COLUMNS*32*8);
  Serial.print("xcursor: "); Serial.print(xcursor);
  Serial.print(", ycursor: "); Serial.print(ycursor);
}

void showCursor(boolean isenable) {
  static boolean cursorToggle;
  static unsigned long last_cursorToggle;
  if ( (millis()-last_cursorToggle)>500 && isenable) {
     last_cursorToggle = millis();
     cursorToggle = !cursorToggle;
     if (cursorToggle) {
        matrix.setPixelColor(cursorNum, NEO_RED);
     } else {
        matrix.setPixelColor(cursorNum, 0);
     }
   
     //matrix.setTextSize(1);
     //matrix.setCursor(xcursor-2,ycursor-5);
     //matrix.print(".");
     matrix.show();
     
     //Serial.print(xcursor); Serial.print(","); Serial.println(ycursor);
  }
}

void deletePreviousCursor() {
  matrix.setTextColor(0);
  matrix.setTextSize(1);
  matrix.print(".");
  matrix.show();
  delay(10);
}
