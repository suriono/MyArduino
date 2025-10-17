#include <TinyGPS.h>
#include <Arduino_JSON.h>
#include <BH1750.h>        // Light sensor
#include <Wire.h>          // D1=SCL, D2=SDA for NodeMCU
#include <SoftwareSerial.h>
#include <Adafruit_NeoMatrix.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include "certs.h"

X509List cert(cert_DigiCert_Global_Root_CA);     // HTTPs certificate

#define PIN_TOP_SIGN 0    // 0=D3 
#define TILE_COLUMNS 1
#define TILE_ROWS    1
#define NEO_RED      9109504
#define WIDTH        27
#define HEIGHT       8
#define RXPIN_GPS    2    // 2=D4
#define TXPIN_GPS   -1
#define PERIOD_SPEED_LIMIT 10000    // period in mS to request speed limit from TomTom
#define MIN_SPEED_LIMIT   0.001            // minimum speed to display speed limit

BH1750 lightMeter;
SoftwareSerial Ss (RXPIN_GPS, TXPIN_GPS);

 // Use Arduino Due because SRAM is high, 96KB, Mega is 8KB
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(WIDTH, HEIGHT, TILE_COLUMNS, TILE_ROWS, PIN_TOP_SIGN,
  NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_ZIGZAG +
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);
int Neomatrix_Brightness = 20;
//void Neopixel_Initial();

TinyGPS gps;
static void smartdelay(unsigned long ms);
static float print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);
//int Speed_MPH;
//JSONVar myJSON;
int speedLimit_mph;
void wifi_begin();
int get_Speed_Limit();
void Neopixel_Initial();

// ============================== SETUP =====================================

void setup() {
  Serial.begin(57600);
  Ss.begin(9600);
  Wire.begin();
  WiFi.mode(WIFI_STA);
  wifi_begin();        // contain Wi-Fi SSID and password, hidden for security

  //lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE);
  lightMeter.begin();
  Neopixel_Initial();

  //for( int nn = 0; nn<3 ; nn++ ) {
  //  speedLimit_mph = get_Speed_Limit();
  //  Serial.print("===== Final mph: "); Serial.println(speedLimit_mph);
  //  delay(5000);
  //}
}

void loop() {
  //while (!lightMeter.measurementReady(true)) { // until light meter is ready
  //   yield();
  //}
  int lux = int(lightMeter.readLightLevel());
  int lux_limit = constrain(lux, 0, 8000);
  Neomatrix_Brightness = map(lux_limit, 0, 8000, 3,255);
  //Serial.print("Light: "); Serial.print(lux); Serial.print(" lx "); 
  //Serial.print("Brightness: "); Serial.print( Neomatrix_Brightness); Serial.print(",");
  matrix.setBrightness(Neomatrix_Brightness); 
  
  //lightMeter.configure(BH1750::ONE_TIME_HIGH_RES_MODE);
  
  GPS_refresh();
  
}
