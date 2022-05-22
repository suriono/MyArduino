#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>

#define NEOPIXEL_PIN    4  // D2 for NodeMCU

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(32, 8, NEOPIXEL_PIN,
  NEO_MATRIX_TOP  + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + 
  NEO_MATRIX_ZIGZAG, NEO_GRB + NEO_KHZ800);

//#ifndef STASSID
//#define STASSID "philip"
//#define STAPSK  "fern5077266123"
//#endif

//const char* ssid = STASSID;
//const char* password = STAPSK;
const char* host = "OTA-FRONT-NEOMATRIX";

const int ESP_BUILTIN_LED = 2;

void setup() {
  Serial.begin(115200);

  Serial.println("Booting");
  WiFi.mode(WIFI_STA);

  wifi_begin();

  ArduinoOTA.setHostname(host);
  ArduinoOTA.onStart([]() { // switch off all the PWMs during upgrade
  });

  ArduinoOTA.onEnd([]() { // do a fancy thing with our board led at end
  });

  ArduinoOTA.onError([](ota_error_t error) {
    (void)error;
    ESP.restart();
  });

  /* setup the OTA server */
  ArduinoOTA.begin();
  Serial.println("Ready");

  delay(2000);   // so no big surge of current at the beginning
  matrix.begin(); 
  matrix.setTextWrap(true); 
  matrix.setBrightness(10);  // from 0 to 255
  matrix.setTextColor(matrix.Color(178, 0, 0 ));
  matrix.fillScreen(matrix.Color(0, 0, 100));
  //matrix.setCursor(0, 0);
  matrix.print(F("zzzzz"));
  //matrix.drawPixel(10, 3, matrix.Color(178, 248, 203 )); 
  //matrix.drawPixel(10, 4,matrix.Color(178, 2, 203 )); 
 
  matrix.show(); 
}

void loop() {
  ArduinoOTA.handle();
}
