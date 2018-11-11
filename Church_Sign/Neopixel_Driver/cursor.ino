byte xcursor, ycursor;

void getCursorNum() {
  cursorNum = xcursor * 8 + (xcursor % 2)*7;
}
