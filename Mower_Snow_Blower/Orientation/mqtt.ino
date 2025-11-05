void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial

    mqtt_client.subscribe(MQTT_TOPIC , [](const String & yaw) { 
       Serial.print("Yaw: "); Serial.println(yaw);
  //    speedLimit_mph = speed_Limit.toInt();
       //int speed_int = int(speed_Limit)
       //TomTom_speed_Limit(speed_Limit);
    });
    mqtt_client.publish(MQTT_TOPIC, "0", true);
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
 // myJSON["waypoints"] = "44.742127,-93.194999:44.745049,-93.194981";
  mqtt_client.publish(MQTT_TOPIC, String(Yaw), true); 
 // client.publish("gps/speedlimit", "39", true);
}

void MQTT_publish_string(String msg) {
 // mqtt_client.publish(MQTT_TOPIC, msg, true);
    myJSON["waypoints"] = "44.742127,-93.194999:44.745049,-93.194981";
  mqtt_client.publish(MQTT_TOPIC, JSON.stringify(myJSON), true); 
}
