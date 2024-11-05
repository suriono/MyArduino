void json_Process() {
  if ( myJSON.hasOwnProperty("cmd")) {
    json_cmd( (String) myJSON["cmd"]);
  }
}

// ====================== JSON cmd ======================

void json_cmd(String msg) {
  //Serial.print("JSON cmd: "); Serial.println(msg);
  if (msg == "get_gps") {
    send_coordinates();
  } else if (msg == "get_rtk") {
    send_rtk();
  }
}
