#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Arduino_JSON.h>

#define PIN_TOP_SIGN 2
#define TILE_COLUMNS 6
#define TILE_ROWS    5
#define NEO_RED      9109504

 // Use Arduino Due because SRAM is high, 96KB, Mega is 8KB
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, TILE_COLUMNS, TILE_ROWS, PIN_TOP_SIGN,
  NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_ZIGZAG +
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);

JSONVar jsonObject;
 
void setup() {
  Serial.begin(57600);
  SerialUSB.begin(115200);
  matrix.begin();
  matrix.setTextWrap(false); 
  matrix.setBrightness(5);  // from 0 to 255
  //matrix.setTextSize(1);
  //matrix.fillScreen(0); 
  matrix.setTextColor(matrix.Color(0, 0, 200));
  //matrix.print(F("St"));
  //matrix.show();
}

int x = 0;
int nbright = 1;
int color_cycle = 0;
int bright_inc = 10;

void loop() {
  static unsigned long count = 0;
  char c;
  if (SerialUSB.available()) {
     //char input[200];
     String input;
     //int cc = 0;
     while(SerialUSB.available()) {
       c = SerialUSB.read();
       //input[cc]= c;  
       input += c;
       //Serial.print(c);
       //cc++;
     }
     jsonObject = JSON.parse(input);
     if (JSON.typeof(jsonObject) == "undefined") {
        Serial.println("Parsing input failed!");
     } else {
        Command_Run();
     }
  }
}

// ================= Serial Event =================
