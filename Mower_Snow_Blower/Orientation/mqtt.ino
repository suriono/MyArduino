void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial

   // mqtt_client.subscribe(MQTT_TOPIC , [](const String & yaw) { 
  //     Serial.print("Yaw: "); Serial.println(yaw);
 //   });
    mqtt_client.publish(MQTT_TOPIC_YAW, "0", true);
    mqtt_client.publish(MQTT_TOPIC_COUNT, "-1", true);
  //  MQTT_publish();
  // Publish a message to "mytopic/test"
 // client.publish("testTopic", "This is a message"); // You can activate the retain flag by setting the third parameter to true

  // Execute delayed instructions
 // client.executeDelayed(5 * 1000, []() {
 //   client.publish("testTopic", "This is a message sent 5 seconds later");
 // });
}

// ================= Publish GPS path =======================

void MQTT_publish() {
  static int count_publish=0;
 // myJSON["waypoints"] = "44.742127,-93.194999:44.745049,-93.194981";
  mqtt_client.publish(MQTT_TOPIC_YAW  , String(Yaw), true); 
  mqtt_client.publish(MQTT_TOPIC_COUNT, String(count_publish++), true);
   
 // client.publish("gps/speedlimit", "39", true);
}
