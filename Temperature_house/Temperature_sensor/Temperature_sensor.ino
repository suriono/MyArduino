#include <OneWire.h>
#include <TM1637Display.h>
#include <SoftwareSerial.h>

OneWire  ds(10);  // on pin 10 of temperature sensor (a 4.7K resistor if necessary)
// Module connection pins (Digital display Pins)
#define CLK 2
#define DIO 3
#define PIN_RELAY_FAN 5

TM1637Display display(CLK, DIO);
SoftwareSerial TempSerial(0, 9); // RX, TX for read temp
#define INTERVAL_TO_NODEMCU 3600000   // interval (ms) sending to NodeMCU
//#define INTERVAL_TO_NODEMCU 10000   // for testing interval (ms) sending to NodeMCU


#define LOW_THRESHOLD  30
#define HIGH_THRESHOLD 33
boolean Fan_OnOff = false;    // Off by default
unsigned long last_time;

// Constant pointers to constant digital data
#define MAXNUMDIGITS 4
byte digitCodes[MAXNUMDIGITS];

// ============= Setup =========================

void setup(void) {
  Serial.begin(9600);
  TempSerial.begin(9600);
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff }; // all on
  display.setBrightness(0x02);
  display.setSegments(data);

  //char inchar[5] = "yesz";
  setChars("test", MAXNUMDIGITS);
  //Digital_Encode( inchar);

  pinMode(PIN_RELAY_FAN, OUTPUT);
  digitalWrite(PIN_RELAY_FAN, !Fan_OnOff);
}
  
// ================ Loop =======================

void loop(void) {
  static boolean fan_last_OnOf = false;
  static unsigned long last_time_serial_send;
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  
  if ( !ds.search(addr)) {
    //Serial.println("No more addresses.");
    //Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }

  /*
  Serial.print("ROM =");
  for( i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }
  */

  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return;
  }
  //Serial.println();
 
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      //Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      return;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  //Serial.print("  Data = ");
  //Serial.print(present, HEX);
  //Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    //Serial.print(data[i], HEX);
    //Serial.print(" ");
  }
  //Serial.print(" CRC=");
  //Serial.print(OneWire::crc8(data, 8), HEX);
  //Serial.println();

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  //fahrenheit = celsius * 1.8 + 32.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  //TempSerial.print("temp");

  if ( (millis() - last_time_serial_send) > INTERVAL_TO_NODEMCU) {
    //TempSerial.print(">temp=" + String(int(celsius)) + "<");
    //char buff[10];
    //sprintf (buff, sizeof(buff), "%f", celsius);
    TempSerial.print(">temp=" + String(celsius) + "<");
    Serial.println("Send to NodeMCU:" + String(celsius));
    last_time_serial_send = millis();
  }
  
  for(int k = 0; k < 4; k++) { // clear digital display
    data[k] = 0;
  }
  display.setSegments(data);

  data[0] = display.encodeDigit(int(celsius)/10);
  data[1] = display.encodeDigit(int(celsius)%10);
  data[2] = SEG_A|SEG_B|SEG_F|SEG_G;  // degree
  data[3] = 0b00111001;  // letter C ;
  display.setSegments(data);

  delay(2000);
  //setChars("fan", MAXNUMDIGITS);
  delay(2000);
  //Serial.print(fahrenheit);
  //Serial.println(" Fahrenheit");
  if (celsius > HIGH_THRESHOLD) {
    Fan_OnOff = true;
    last_time = millis();   // last detected switch
  } else if (celsius < LOW_THRESHOLD) {
    last_time = millis();   // last detected switch
    Fan_OnOff = false;
  }

  digitalWrite(PIN_RELAY_FAN, !Fan_OnOff);
  if (Fan_OnOff) {
    Serial.println("Fan: ON");
    //setChars("on", MAXNUMDIGITS);
  } else {
    Serial.println("Fan: OFF");
    //setChars("off", MAXNUMDIGITS);
  }

  if (Fan_OnOff != fan_last_OnOf) {
    Serial.println("    Switch changed");
  }
  fan_last_OnOf = Fan_OnOff;
  
}

// ==============Not used ===== Digital Display Encoder ==============
/*
void Digital_Encode(char* ch) {
  uint8_t disdata[4] = { 0,0,0,0};

  for (byte tt=0; tt<4 ; tt++) {
    disdata[tt] = Digital_Encode_Letter(ch[tt]);
  }
 
  display.setSegments(disdata);
}

uint8_t Digital_Encode_Letter(char var) {
  // SEG_A : top
  // SEG_B : top right
  // SEG_C : bottom right
  // SEG_D : bottom
  // SEG_E : bottom left
  // SEG_F : top left
  // SEG_G : middle
  switch (var) {
    case 'c':
      return SEG_A | SEG_D | SEG_E  | SEG_F;  break;
    case 'e':
      return SEG_A | SEG_D | SEG_E  | SEG_F | SEG_G;  break;
    case 'h':
      return SEG_B | SEG_C | SEG_E  | SEG_F | SEG_G;  break;
    case 'l':
      return SEG_C | SEG_D | SEG_E  | SEG_F ;  break;
    default: 
      return 0;
    break;
  }
}
*/

// =================================================

static const byte digitCodeMap[] = {
  //     GFEDCBA  Segments      7-segment map:
  B00111111, // 0   "0"          AAA
  B00000110, // 1   "1"         F   B
  B01011011, // 2   "2"         F   B
  B01001111, // 3   "3"          GGG
  B01100110, // 4   "4"         E   C
  B01101101, // 5   "5"         E   C
  B01111101, // 6   "6"          DDD
  B00000111, // 7   "7"
  B01111111, // 8   "8"
  B01101111, // 9   "9"
  B01110111, // 65  'A'
  B01111100, // 66  'b'
  B00111001, // 67  'C'
  B01011110, // 68  'd'
  B01111001, // 69  'E'
  B01110001, // 70  'F'
  B00111101, // 71  'G'
  B01110110, // 72  'H'
  B00000110, // 73  'I'
  B00001110, // 74  'J'
  B01110110, // 75  'K'  Same as 'H'
  B00111000, // 76  'L'
  B00000000, // 77  'M'  NO DISPLAY
  B01010100, // 78  'n'
  B00111111, // 79  'O'
  B01110011, // 80  'P'
  B01100111, // 81  'q'
  B01010000, // 82  'r'
  B01101101, // 83  'S'
  B01111000, // 84  't'
  B00111110, // 85  'U'
  B00111110, // 86  'V'  Same as 'U'
  B00000000, // 87  'W'  NO DISPLAY
  B01110110, // 88  'X'  Same as 'H'
  B01101110, // 89  'y'
  B01011011, // 90  'Z'  Same as '2'
  B00000000, // 32  ' '  BLANK
  B01000000, // 45  '-'  DASH
};

// =====================================================

const byte * const alphaCodes = digitCodeMap + 10;
const byte * const numeralCodes = digitCodeMap;
#define BLANK_IDX 36 // Must match with 'digitCodeMap'
#define DASH_IDX 37

void setChars(char str[], byte numDigits)
{
  Serial.println(str);
  for (byte digit = 0; digit < numDigits; digit++) {
    digitCodes[digit] = 0;
  }

  for (byte digitNum = 0; digitNum < numDigits; digitNum++) {
    char ch = str[digitNum];
    if (ch == '\0') break; // NULL string terminator
    if (ch >= '0' && ch <= '9') { // Numerical
      digitCodes[digitNum] = numeralCodes[ch - '0'];
    }
    else if (ch >= 'A' && ch <= 'Z') {
      digitCodes[digitNum] = alphaCodes[ch - 'A'];
    }
    else if (ch >= 'a' && ch <= 'z') {
      digitCodes[digitNum] = alphaCodes[ch - 'a'];
    }
    else if (ch == ' ') {
      digitCodes[digitNum] = digitCodeMap[BLANK_IDX];
    }
    else {
      // Every unknown character is shown as a dash
      digitCodes[digitNum] = digitCodeMap[DASH_IDX];
    }
    Serial.println(digitCodes[digitNum]);
  }
  display.setSegments(digitCodes);
}


