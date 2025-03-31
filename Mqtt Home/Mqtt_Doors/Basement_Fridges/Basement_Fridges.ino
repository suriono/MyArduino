
#include <Arduino_JSON.h>
#include "EspMQTTClient.h"
#include <ArduinoOTA.h>
#include "password.h"       // hidden for security, for all user names and passwords


// Comment not used MQTT Topic, uncomment which door to upload
//#define MQTT_Topic    "door/basement/freezer"      // Uncomment this for different MQTT topic, for the basement freezer
//const char* ota_host = "OTA-FREEZER";              // Over-The-Air code upload/update

#define MQTT_Topic    "door/basement/topfridge"  // Uncomment this for different MQTT topic, for the basement top fridge (freezer) 
const char* ota_host = "OTA-TOP-FRIDGE";         // Over-The-Air code upload/update

//#define MQTT_Topic    "door/basement/bottomfridge" // Uncomment this for different MQTT topic, Bottom fridge
//const char* ota_host = "OTA-BOTTOM-FRIDGE";      // Over-The-Air code upload/update


JSONVar myJSON, statusJSON;
EspMQTTClient client( WIFI_SSID, WIFI_PASSWD, "192.168.0.122", MQTT_USER, MQTT_PASSWD, ota_host,1883); // passwords are hidden
boolean isPublished = true;
boolean last_state = true;
boolean new_state = false;

#define MAGNETIC_PIN 5   // D1
#define TIMER_TIME   120 // number of countdown before sending alerts

void setup() {
    pinMode(MAGNETIC_PIN, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    client.enableMQTTPersistence();
    //client.setMqttReconnectionAttemptDelay(4660);
    //client.setKeepAlive(60);
    //client.enableLastWillMessage(MQTT_Topic, "{\"status\":\"offline\"}",true);

    // Setting up update this firmware via Wi-Fi
    ArduinoOTA.setHostname(ota_host);
    ArduinoOTA.onStart([]() { // switch off all the PWMs during upgrade
    });
    ArduinoOTA.onEnd([]() { // do a fancy thing with our board led at end
    });
    ArduinoOTA.onError([](ota_error_t error) {
      (void)error;
      ESP.restart();
    });
    ArduinoOTA.begin();
}

// ===================================================================

void publish_MQTT(bool new_state) {
   isPublished = client.isConnected();
   if ( isPublished ) {
      last_state = new_state;
      if ( new_state ) {
         myJSON["countDown_Alarm"] = TIMER_TIME;
      } else {
         myJSON["countDown_Alarm"] = 0;
      }
      client.publish(MQTT_Topic, JSON.stringify(myJSON), true); 
     // client.setKeepAlive(5);
   }
}

// ===================================================================

void loop() {
  static unsigned long last_time = millis();
  static bool LED_OnOff;
  static unsigned long LED_last_time = millis();
  static bool last_connected;
  bool isconnected = false;

  ArduinoOTA.handle();
  client.loop();

  if ( (millis() - last_time) > 500) {
    last_time = millis();

    new_state = digitalRead(MAGNETIC_PIN);

    if ( new_state != last_state) {
      publish_MQTT( new_state);
    }
  }

  if ( (millis()-LED_last_time) > 1000) {   // LED control only
     LED_last_time = millis();
     if (new_state) { // LED blinks when door is open
        LED_OnOff = !LED_OnOff;
        digitalWrite(LED_BUILTIN, LED_OnOff);
     } else {         // if door is closed, check if connected
        isconnected = client.isConnected();
        if (isconnected != last_connected) {
           digitalWrite(LED_BUILTIN, isconnected); // LED solid ON when connected
           last_connected = isconnected;
        } else if (isconnected && !LED_OnOff) {  // connected by was On from opening the door
           LED_OnOff = !LED_OnOff;
           digitalWrite(LED_BUILTIN, LED_OnOff);
        }
     }
  }
}

// ===================================================================

void onConnectionEstablished()  {   // when connected or reconnected
   if ( !isPublished ) {  // if failed to publish it earlier, try again
      publish_MQTT( new_state);
   }
}
