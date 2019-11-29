
// ================= Init ==================

void matrix_init() {
  Serial.begin(57600);
  matrix.begin();
  matrix.setTextWrap(true);
  matrix.setBrightness(8);
  matrix.setTextColor(colors[0]);
 
}
