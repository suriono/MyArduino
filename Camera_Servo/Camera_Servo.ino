 
#include <Servo.h>
#include <Arduino.h>
#include "password.h"   // hidden for security
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

Servo myservo;  // create servo object to control a servo
#define CENTER_ANGLE 85

AsyncWebServer server(80);

//const char* ssid     = "xxxxxxxxxxxxxxxxxxxx";
//const char* password = "xxxxxxxxxxxxxxxxxxxx";

//const char* PARAM_MESSAGE = "angle";

int count = 0;
static int last_angle = 0;

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Fail to control camera servo " + String(count) );
    Serial.println("failllll");
    count++;
}

// =============================== Setup =======================================
void setup() {
//    myservo.attach(4,544, 2600);  // attaches the servo on pin 9 to the servo object
   // myservo.attach(0,600,2600);  // attaches the servo on pin 9 to the servo object

    Serial.begin(57600);
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID_NAME, WIFI_PASSWORD);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Servo_Run(CENTER_ANGLE);       // initial angle in the center

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "angle=" + String(last_angle));
    });

    // Send a GET request to <IP>/get?message=<message>
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String message;
        if (request->hasParam("angle")) {
            message = request->getParam("angle")->value();
        } else {
            message = "Failed http get with request";
        }
        Serial.print("HTTP_GET:"); Serial.println(message);
        request->send(200, "text/plain", "Hello, GET: " + message);
    });

    // Send a POST request to <IP>/post with a form field message set to <message>
    server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
        String message;
        if (request->hasParam("angle", true)) {
            String angle = request->getParam("angle", true)->value();
            Servo_Run(angle.toInt());
            message = "Received command angle : " + angle + " degree";
            last_angle = angle.toInt();
        //} else if (request->hasParam("getangle", true)) {
         //   message = "angle=" + String(last_angle);
        } else {
            message = "Failed to post control camera servo";
        }
        Serial.println(message);
        count++;
        request->send(200, "text/html", message );
    });

    server.onNotFound(notFound);

    server.begin();
    myservo.write(0);
}

// =============================== Loop =========================================
void loop() {
  
  //Servo_Run();
}
