
#include <Arduino_JSON.h>
#include "EspMQTTClient.h"
#include "password.h"

// Comment not used MQTT Topic
//#define MQTT_Topic    "door/basement/freezer"        // Uncomment this for different MQTT topic, for the basement freezer
//#define MQTT_Topic    "door/basement/topfridge"    // Uncomment this for different MQTT topic, for the basement top fridge (freezer) 
#define MQTT_Topic    "door/basement/bottomfridge" // Uncomment this for different MQTT topic, Bottom fridge


JSONVar myJSON;
EspMQTTClient client( WIFI_SSID, WIFI_PASSWD, "192.168.0.122", MQTT_USER, MQTT_PASSWD, "uzbasementfreezer",1883); // passwords are hidden

#define MAGNETIC_PIN 5   // D1
#define TIMER_TIME   120 // number of countdown before sending alerts

void setup() {
    pinMode(MAGNETIC_PIN, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  static unsigned long last_time = millis();
  uint16_t color_text;
  static boolean last_state = true;
  boolean new_state = false;
  static bool LED_OnOff;
  static unsigned long LED_last_time = millis();

  client.loop();

  if ( (millis() - last_time) > 200) {
    last_time = millis();

    new_state = digitalRead(MAGNETIC_PIN);

    if ( new_state != last_state) {
      last_state = new_state;
      if ( new_state ) {
         myJSON["countDown_Alarm"] = TIMER_TIME;
      } else {
         myJSON["countDown_Alarm"] = 0;
      }
      client.publish(MQTT_Topic, JSON.stringify(myJSON)); 
    }
  }

  if ( (millis()-LED_last_time) > 500 && new_state) { // LED blinks when door is open
     LED_last_time = millis();
     LED_OnOff = !LED_OnOff;
     digitalWrite(LED_BUILTIN, LED_OnOff);
  }
}

void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
  //client.subscribe("testTopic", [](const String & payload) {
  //  Serial.println(payload);
  //});

  // Subscribe to "mytopic/wildcardtest/#" and display received message to Serial
  //client.subscribe("mytopic/wildcardtest/#", [](const String & topic, const String & payload) {
   // Serial.println("(From wildcard) topic: " + topic + ", payload: " + payload);
  //});

  // Publish a message to "mytopic/test"
  //client.publish("door/basement/freezer", ""); // You can activate the retain flag by setting the third parameter to true

  // Execute delayed instructions
  //client.executeDelayed(5 * 1000, []() {
  //  client.publish("testTopic", "This is a message sent 5 seconds later");
  //});
}
