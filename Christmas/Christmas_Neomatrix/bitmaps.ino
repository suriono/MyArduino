const boolean heart[] = {       // Heart bitmap
  0, 0, 1, 0, 0, 0, 1, 0, 0,
  0, 1, 1, 1, 0, 1, 1, 1, 0,
  1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 1, 1, 1, 1, 1, 1, 1, 0,
  0, 0, 1, 1, 1, 1, 1, 0, 0,
  0, 0, 0, 1, 1, 1, 0, 0, 0,
  0, 0, 0, 0, 1, 0, 0, 0, 0
};

const boolean stPatrick[] = {       // Heart bitmap
  0, 0, 0, 0, 0, 1, 1, 0, 0,
  0, 1, 1, 0, 1, 1, 1, 1, 0,
  1, 1, 1, 0, 1, 1, 1, 1, 0,
  1, 1, 1, 1, 1, 1, 1, 0, 0,
  0, 1, 0, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 0,
  0, 0, 1, 1, 0, 1, 1, 1, 0,
  0, 1, 1, 0, 0, 1, 1, 1, 0
};

const boolean star[] = {       // Heart bitmap
  0, 0, 0, 0, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 0, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 1, 1, 1, 1, 1, 0, 0,
  0, 0, 0, 0, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 0, 0, 0,
  0, 1, 1, 0, 0, 0, 1, 1, 0,
  1, 0, 0, 0, 0, 0, 0, 0, 1
};


// =============== Draw Bitmap =============

void Draw_Bitmap(boolean* bitmap, int col, int xoffset, uint16_t colorm) {
  int x, y;
  for (int nn = 0; nn < 8*col; nn++) {
      x = nn % col + xoffset;
      y = nn / col;
      //matrix.drawPixel(x, y, matrix.Color(200,0,0) * bitmap[nn]); 
      matrix.drawPixel(x, y, colorm * bitmap[nn]); 
  }
}

// =================================================
void Neomatrix_scrollbitmap(char pic[], boolean isBlink) {
  boolean* bitmap_source;
  byte bitmap_width;
  uint16_t mcolor1, mcolor2;

  if (strcmp(pic, "heart") == 0) {
    bitmap_width = 9;
    bitmap_source = heart;
    mcolor1 = matrix.Color(0,200,0);
    mcolor2 = matrix.Color(0,200,0);
  } else if (strcmp(pic, "stPatrick") == 0) {
    bitmap_width = 9;
    bitmap_source = stPatrick;
    mcolor1 = matrix.Color(0,250,0);
    mcolor2 = matrix.Color(0,250,0);
  } else if (strcmp(pic, "star") == 0) {
    bitmap_width = 9;
    bitmap_source = star;
    mcolor1 = matrix.Color(10,10,230);
    mcolor2 = matrix.Color(240,0,0);
  }

  //matrix.fillScreen(0); 
  //Draw_Bitmap(bitmap_source, bitmap_width, -23+32-bitmap_width,  mcolor);
  //matrix.show();
  //delay(1000);

  for (int x=0 ; x < 32-8; x++) {
    matrix.fillScreen(0); 
    if (isBlink) {
      matrix.show(); delay(100);     // blinking
    }

    Draw_Bitmap(bitmap_source, bitmap_width, -x+32-bitmap_width, mcolor1);
    matrix.show();
    delay(20);
  }
  for (int x=32-8 ; x > 0; x--) {
    matrix.fillScreen(0); 
    if (isBlink) {
      matrix.show(); delay(100);     // blinking
    }

    Draw_Bitmap(bitmap_source, bitmap_width, -x+32-bitmap_width, mcolor2);
    matrix.show();
    delay(20);
  }

  
  for (int x=0 ; x < 32-8; x++) {
    matrix.fillScreen(0); 
    if (isBlink) {
      matrix.show(); delay(100);     // blinking
    }

    Draw_Bitmap(bitmap_source, bitmap_width, -x+32-bitmap_width, mcolor1);
    matrix.show();
    delay(20);
  }
  for (int x=32-8 ; x > -9; x--) {
    matrix.fillScreen(0); 
    if (isBlink) {
      matrix.show(); delay(100);     // blinking
    }

    Draw_Bitmap(bitmap_source, bitmap_width, -x+32-bitmap_width, mcolor2);
    matrix.show();
    delay(20);
  }
}

// =================================================
