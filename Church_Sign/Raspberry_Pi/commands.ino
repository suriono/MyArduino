void Command_Run() {
  String Cmd = JSON.stringify(jsonObject["cmd"]); Cmd.replace("\"","");
  Serial.print(Cmd);
  Serial.print("========");
  Serial.println();
  if (Cmd.equals("DeleteAll")) {
     Delete_All();
  } else if (Cmd.equals("TestConnection")) {
     Serial.print("test connection OK");
     SerialUSB.println("OK");
  } else if (Cmd.equals("SetCursor")) {
     Set_Cursor();
  } else if (Cmd.equals("SetText")) {
     Set_Text();
  
  }
}

// ============== Delete All ===============
void Delete_All() {
  matrix.fillScreen(0);
  matrix.show();
}

// ============== Set Cursor ===============
void Set_Cursor() {
  String tmpx = JSON.stringify(jsonObject["X"]); tmpx.replace("\"",""); 
  String tmpy = JSON.stringify(jsonObject["Y"]); tmpy.replace("\"",""); 
  int x = tmpx.toInt();
  int y = tmpy.toInt();
  matrix.setCursor(x,y);
  //matrix.print(F("Uz"));
  //matrix.show();
 // Serial.println(jsonObject["X"]);
}

// ============== Set Text ===============
void Set_Text() {
  String tmpx = JSON.stringify(jsonObject["X"]); tmpx.replace("\"",""); 
  String tmpy = JSON.stringify(jsonObject["Y"]); tmpy.replace("\"",""); 
  int x = tmpx.toInt();
  int y = tmpy.toInt();
  matrix.setCursor(x,y);
  
  String intext = JSON.stringify(jsonObject["Text"]); intext.replace("\"",""); 
  matrix.print(intext);
  matrix.show();
 // Serial.println(jsonObject["X"]);
}
