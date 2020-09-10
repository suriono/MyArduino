StaticJsonDocument<200> doc;
void JSON_process(String InStr) {
  Serial.println(InStr); 
  //DynamicJsonDocument doc(200);
  DeserializationError error = deserializeJson(doc, InStr);
  
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  if (InStr.indexOf('"d:"')) {
    Delay_Time = doc["d"];
    Serial.println("delay: " + String(Delay_Time));
  }

  if (InStr.indexOf('"a:"')) {
    Adjacent = doc["a"];
    Serial.println("Adjacent: " + String(Adjacent));   
  }

  if (InStr.indexOf('"m:"')) {
    Mode = doc["m"];
    Serial.println("Mode: " + String(Mode));   
  }
  
  if (InStr.indexOf('"c:"')) {
    Color_Set = doc["c"];
    Serial.println("Color: " + String(Color_Set));  
    /*
    JsonArray colors = doc["c"].as<JsonArray>();
    Num_Colors = 0;
    for (JsonVariant v : colors) {
      Serial.print(v[0].as<int>()); Serial.print(",");
      Serial.print(v[1].as<int>()); Serial.print(",");
      Serial.println(v[2].as<int>());
      Colors[Num_Colors] = CRGB(v[1].as<int>(),v[0].as<int>(),v[2].as<int>());
      Num_Colors++;
    }*/
  }

  for (byte een=0; een<30; een++) {
    if (een < InStr.length()) {
      EEPROM.put(een, InStr.charAt(een));
    } else {
      EEPROM.put(een, ' ');
    }
  }
}
