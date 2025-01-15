#include "EspMQTTClient.h"
#include "password.h"

EspMQTTClient client( WIFI_SSID, WIFI_PASSWD, "192.168.0.122", MQTT_USER, MQTT_PASSWD, "uzbasementfreezer",1883); // passwords are hidden

#define MAGNETIC_PIN 14 // D5 RXD0


void setup() {
    pinMode(MAGNETIC_PIN, INPUT_PULLUP);
}

void loop() {
  static unsigned long last_time = millis();
  static String last_String = "";
  String new_String;
  uint16_t color_text;

  client.loop();

  if ( (millis() - last_time) > 2000) {
    last_time = millis();

    if ( digitalRead(MAGNETIC_PIN) ) {
      new_String = "OPEN";
//      color_text = tft.color565(250, 0 , 0);
    } else {
      new_String = "CLOSE";
//      color_text = tft.color565(0, 250 , 0);
    }
//
//    if (new_String != last_String) {
      client.publish("door/basement/freezer", new_String); // You can activate the retain flag by setting the third parameter to true

//      tft.setCursor(150, 0);
//      tft.setTextColor(ILI9341_BLACK);
//      tft.print(last_String);
//      tft.setCursor(150, 0);
//      tft.setTextColor(color_text);    
//      tft.print(new_String);
      last_String = new_String;
  //  }
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
