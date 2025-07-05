void Neopixel_Initial() {
  //delay(5000);   // so no big surge of current at the beginning
  matrix.begin();
  matrix.setTextWrap(false); 
  matrix.setBrightness(Neomatrix_Brightness);  // from 0 to 255
  
  matrix.setTextSize(1);
  matrix.setTextColor(matrix.Color(0, 255, 0));

  Neopixel_Text("Wait", 0);
  matrix.show();
  delay(5000);
}

// ==================== Text with mirror image =====================

void Neopixel_Text(String msg, int offsetx) {
  matrix.setBrightness(Neomatrix_Brightness); 
  matrix.setCursor(offsetx,0);
  //matrix.fillScreen(0); 
  matrix.print(msg);
  //matrix.show();
}

// =============== Mirror pixels for projection to windshield =======
    
void Neopixel_Mirror() {
  // Mirror the display
  uint16_t pixno;
  uint32_t tempL,tempR, r,g,b;
    
  for (int x = 0; x < WIDTH/2; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      // swapping pixel colors, 1st one
      if ( x%2 == 0) {  // even number
        pixno = x*HEIGHT + y;
      } else {
        pixno = x*HEIGHT + HEIGHT-y-1;
      }
      tempL = matrix.getPixelColor(pixno);
      //Serial.print("x:"); Serial.print(x); Serial.print(",y:"); Serial.print(y);
      //Serial.print(",pixno:"); Serial.print(pixno);Serial.print(",color:");Serial.print(tempL);
     
      // swapping pixel colors, 2nd one
      if ( (WIDTH-x-1)%2 == 0) {  // even number
        pixno = (WIDTH-x-1)*HEIGHT + y;
      } else {
        pixno = (WIDTH-x-1)*HEIGHT + HEIGHT-y-1;
      }
      tempR = matrix.getPixelColor(pixno);
      //Serial.print(",pixno2:"); Serial.print(pixno); Serial.print("color:"); Serial.println(tempR);
      b = tempR & 255;
      g = (tempR >> 8) & 255;
      r = (tempR >> 16) & 255;
      matrix.drawPixel(x, y, matrix.Color(r,g,b)); // matrix.Color(0,0,tempR));
    
      b = tempL & 255;
      g = (tempL >> 8) & 255;
      r = (tempL >> 16) & 255;
      matrix.drawPixel(WIDTH - 1 - x, y, matrix.Color(r,g,b));
     
    }
  }
  //matrix.show();
}

// ========================= Blank =======================================

void Neopixel_Blank(){
  matrix.fillScreen(0); 
  matrix.show();
}
