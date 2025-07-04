void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial

    client.subscribe("water/water_softener", [](const String & water_Softener) { 
       Serial.print("Water Softener: "); Serial.println(water_Softener);
       speed_Limit(water_Softener);
    });
    MQTT_publish();
  // Publish a message to "mytopic/test"
 // client.publish("testTopic", "This is a message"); // You can activate the retain flag by setting the third parameter to true

  // Execute delayed instructions
 // client.executeDelayed(5 * 1000, []() {
 //   client.publish("testTopic", "This is a message sent 5 seconds later");
 // });
}

// ================= Publish GPS path =======================

void MQTT_publish() {
  myJSON["waypoints"] = "44.742127,-93.194999:44.745049,-93.194981";
  client.publish(MQTT_Topic, JSON.stringify(myJSON), true); 
  client.publish("gps/speedlimit", "39", true);
}