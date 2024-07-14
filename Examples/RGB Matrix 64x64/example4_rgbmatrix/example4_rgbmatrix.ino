/***************************************************
*
* For 32x32 RGB LED matrix.
*
* @author lg.gang
* @version  V1.0
* @date  2016-10-28
*
* GNU Lesser General Public License.
* See <http://www.gnu.org/licenses/> for details.
* All above must be included in any redistribution
* ****************************************************/

//#include <Adafruit_GFX.h>   // Core graphics library
//#include <RGBmatrixPanel.h> // Hardware-specific library
//
//// If your 32x32 matrix has the SINGLE HEADER input,
//// use this pinout:
//#define CLK 11  // MUST be on PORTB! (Use pin 11 on Mega)
//#define OE  9
//#define LAT 10
//#define A   A0
//#define B   A1
//#define C   A2
//#define D   A3
//// If your matrix has the DOUBLE HEADER input, use:
////#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
////#define LAT 9
////#define OE  10
////#define A   A3
////#define B   A2
////#define C   A1
////#define D   A0
//RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

#include <DFRobot_RGBMatrix.h> // Hardware-specific library
#include <Wire.h>

// If your 32x32 matrix has the SINGLE HEADER input,
// use this pinout:
#define CLK 11  // MUST be on PORTB! (Use pin 11 on Mega)
#define OE  9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3
#define E   A4
#define WIDTH 64
#define HIGH  64
// If your matrix has the DOUBLE HEADER input, use:
//#define CLK 11  // MUST be on PORTB! (Use pin 11 on Mega)
//#define LAT 9
//#define OE  10
//#define A   A3
//#define B   A2
//#define C   A1
//#define D   A0
//RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);
DFRobot_RGBMatrix matrix(A, B, C, D, E, CLK, LAT, OE, false, WIDTH, HIGH);


void setup() {

  matrix.begin();

  // whew!
}

void loop() {
  // fix the screen with red
  matrix.fillRect(0, 0, 32, 32, matrix.Color333(7, 0, 0));
  delay(500);
  // fix the screen with green
  matrix.fillRect(0, 0, 32, 32, matrix.Color333(0, 7, 0));
  delay(500);
  // fix the screen with blue
  matrix.fillRect(0, 0, 32, 32, matrix.Color333(0, 0, 7));
  delay(500);
  // fix the screen with yellow
  matrix.fillRect(0, 0, 32, 32, matrix.Color333(7, 7, 0));
  delay(500);
  // fix the screen with purple
  matrix.fillRect(0, 0, 32, 32, matrix.Color333(7, 0, 7));
  delay(500);
  // fix the screen with cyan
  matrix.fillRect(0, 0, 32, 32, matrix.Color333(0,7, 7));
  delay(500);
  // fix the screen with white
  matrix.fillRect(0, 0, 32, 32, matrix.Color333(3, 3, 3));
  delay(500);

   // fill the screen with 'black'
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  // draw a pixel in solid white
  matrix.drawPixel(0, 0, matrix.Color333(7, 7, 7));
  delay(500);

  // fix the screen with green
  matrix.fillRect(0, 0, 32, 32, matrix.Color333(0, 7, 0));
  delay(500);

  // draw a box in yellow
  matrix.drawRect(0, 0, 32, 32, matrix.Color333(7, 7, 0));
  delay(500);

  // draw an 'X' in red
  matrix.drawLine(0, 0, 31, 31, matrix.Color333(7, 0, 0));
  matrix.drawLine(31, 0, 0, 31, matrix.Color333(7, 0, 0));
  delay(500);

  // draw a blue circle
  matrix.drawCircle(10, 10, 10, matrix.Color333(0, 0, 7));
  delay(500);

  // fill a violet circle
  matrix.fillCircle(21, 21, 10, matrix.Color333(7, 0, 7));
  delay(500);

  // fill the screen with 'black'
  matrix.fillScreen(matrix.Color333(0, 0, 0));

  // draw some text!
  matrix.setCursor(1, 0);    // start at top left, with one pixel of spacing
  matrix.setTextSize(1);     // size 1 == 8 pixels high
  matrix.setTextWrap(false); // Don't wrap at end of line - will do ourselves

  matrix.setTextColor(matrix.Color333(0,7,0));
  matrix.println("green");
  matrix.setTextColor(matrix.Color333(0,7,7));
  matrix.println("robot");

  // print each letter with a rainbow color
  matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.print('3');
  matrix.setTextColor(matrix.Color333(7,4,0));
  matrix.print('2');
  matrix.setTextColor(matrix.Color333(7,7,0));
  matrix.print('x');
  matrix.setTextColor(matrix.Color333(4,7,0));
  matrix.print('3');
  matrix.setTextColor(matrix.Color333(0,7,0));
  matrix.println('2');

  matrix.setTextColor(matrix.Color333(0,7,7));
  matrix.print('*');
  matrix.setTextColor(matrix.Color333(0,4,7));
  matrix.print('R');
  matrix.setTextColor(matrix.Color333(0,0,7));
  matrix.print('G');
  matrix.setTextColor(matrix.Color333(4,0,7));
  matrix.print('B');
  matrix.setTextColor(matrix.Color333(7,0,4));
  matrix.print('*');
  delay(5000);
}
