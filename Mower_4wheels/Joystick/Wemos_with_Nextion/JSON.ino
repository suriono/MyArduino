void JSON_send_key_value(String key, int value) {
   if (!wificlient.connect(HOST_IP, HOST_PORT)) {
      #ifdef DEBUG
         Serial.println("\t,  Connection to robot failed !!!!");
      #endif
      delay(1);
      return;
   }
   wificlient.print("{\"" + key + "\":" + String(value) + "}<");
}
