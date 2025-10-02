#include "EspMQTTClient.h"
#include <Arduino_JSON.h>
#include <TinyGPS.h>
#include <BH1750.h>        // Light sensor
#include <Wire.h>          // D1=SCL, D2=SDA for NodeMCU
#include <SoftwareSerial.h>
#include <Adafruit_NeoMatrix.h>
#include <ESP8266WiFi.h>
#include "password.h"

#define MQTT_Topic "gps/waypoints"
#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#elif __has_include(<WiFiNINA.h>)
#include <WiFiNINA.h>
#elif __has_include(<WiFi101.h>)
#include <WiFi101.h>
#elif __has_include(<WiFiS3.h>)
#include <WiFiS3.h>
#endif
EspMQTTClient client( WIFI_SSID, WIFI_PASSWD, MQTT_IP, MQTT_USER, MQTT_PASSWD, "gpsmqtt",1883); // passwords are hidden for security
JSONVar myJSON;

// ================= NeoMatrix LED stuff ===============================
#define PIN_TOP_SIGN 0    // 0=D3 
#define TILE_COLUMNS 1
#define TILE_ROWS    1
#define NEO_RED      9109504
#define WIDTH        27
#define HEIGHT       8
#define RXPIN_GPS    2    // 2=D4
#define TXPIN_GPS   -1
#define PERIOD_SPEED_LIMIT 5000    // period in mS to request speed limit from TomTom
#define MIN_SPEED_LIMIT   0.001            // minimum speed to display speed limit
 // Use Arduino Due because SRAM is high, 96KB, Mega is 8KB
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(WIDTH, HEIGHT, TILE_COLUMNS, TILE_ROWS, PIN_TOP_SIGN,
  NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_ZIGZAG +
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);
int Neomatrix_Brightness = 20;

// ============= Light sensor ============================
BH1750 lightMeter;
SoftwareSerial Ss (RXPIN_GPS, TXPIN_GPS);

// ======================= GPS stuff =====================
TinyGPS gps;
static void smartdelay(unsigned long ms);
static float print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);
//int Speed_MPH;
//JSONVar myJSON;
int speedLimit_mph = 0;
//void wifi_begin();
//int get_Speed_Limit();
//void Neopixel_Initial();

// ==================================== Setup =================================

void setup() {
  Serial.begin(57600);
  client.enableMQTTPersistence();   // MQTT client

  Ss.begin(9600);                   // Software serial for GPS
  Wire.begin();
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);
  int ncount = 0;
  while (WiFi.status() != WL_CONNECTED && ncount < 5) {
    delay(500);
    Serial.print(".");
    ncount++;
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  //wifi_begin();        // contain Wi-Fi SSID and password, hidden for security

  //lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE);
  lightMeter.begin();
  Neopixel_Initial();

  delay(1000);
  
}

// ==================== Loop ===============================================

void loop() {
  client.loop();

  int lux = int(lightMeter.readLightLevel());
  int lux_limit = constrain(lux, 0, 8000);
  Neomatrix_Brightness = map(lux_limit, 0, 8000, 3,255);
  Serial.print("Light: "); Serial.print(lux); Serial.print(" lx "); 
  Serial.print("Brightness: "); Serial.print( Neomatrix_Brightness); Serial.print(",");
  matrix.setBrightness(Neomatrix_Brightness); 
  GPS_refresh();
}
