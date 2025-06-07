
//boolean door_Basement_Freezer;


//void mqtt_init() {
  // Optional functionalities of EspMQTTClient
//  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
 // client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
 // client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
 // client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true
//}
void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
    
    client.subscribe("door/basement/freezer", [](const String & door_Basement_Freezer) { 
       Door_Number = 0;
       Serial.print("Freezer: "); Serial.println(door_Basement_Freezer);
       tft_basement_freezer(door_Basement_Freezer);  
    });

    client.subscribe("door/basement/topfridge", [](const String & door_Basement_Top_Fridge) { 
       Door_Number = 1;
       Serial.print("Top Fridge: "); Serial.println(door_Basement_Top_Fridge);
       tft_basement_freezer(door_Basement_Top_Fridge);  
    });

    client.subscribe("door/basement/bottomfridge", [](const String & door_Basement_Bottom_Fridge) { 
       Door_Number = 2;
       Serial.print("Bottom Fridge: "); Serial.println(door_Basement_Bottom_Fridge);
       tft_basement_freezer(door_Basement_Bottom_Fridge);  
    });

    client.subscribe("water/water_softener", [](const String & water_Softener) { 
       Serial.print("Water Softener: "); Serial.println(water_Softener);
       tft_water(water_Softener);  
    });
  // Publish a message to "mytopic/test"
 // client.publish("testTopic", "This is a message"); // You can activate the retain flag by setting the third parameter to true

  // Execute delayed instructions
 // client.executeDelayed(5 * 1000, []() {
 //   client.publish("testTopic", "This is a message sent 5 seconds later");
 // });
}
