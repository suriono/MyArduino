#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include "password.h"
#include "certs.h"

X509List cert(cert_DigiCert_Global_Root_CA);

void setup() {
  Serial.begin(57600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println(get_Weather());
}

void loop() {
  // put your main code here, to run repeatedly:

}
