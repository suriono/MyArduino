void write_client() {
  // send a standard http response header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");  // the connection will be closed after completion of the response
  //client.println("Refresh: 10");  // refresh the page automatically every 5 sec
  client.println("Access-Control-Allow-Origin: *");
  client.println();
  //client.println("<!DOCTYPE HTML>");
  //client.println("<html>");

  //client.print("Data received:");
  //client.print(Incoming_Data);
  client.print("OK");

  //client.println("<br/></html>");
}

// ===================================================

void incoming_client() {
    char readchar;
    String readstr, datastr;
    int count = 0;
    bool isStartData = false;
    while (client.connected() && count < 50000) {
      count++;
      if(client.available()) {
        readchar = client.read();
        readstr += readchar;

        if (readstr.indexOf("<endcmd>") > 100) {
           break;
        }
        
        if (isStartData) {
           datastr += readchar;
        } else if(readstr.indexOf("<cmd>") > 100) {
           isStartData = true;
        }
      } else {
        delayMicroseconds(10);  // if it goes too fast
      }
    }
    //Serial.print("Count: "); Serial.println(count);
    //Serial.println("Get incoming:");
    //Serial.print(readstr); Serial.println("---end of incoming");
    //Serial.print(datastr); Serial.println("===");
    Incoming_Data = datastr.substring(0,datastr.indexOf("<endcmd"));
    Serial.print("Command: "); Serial.println(Incoming_Data);
    //Serial1.print(Incoming_Data);
    
    delay(1);
    //write_client();
    //client.flush();
    client.stop();
    Serial.println("client disonnected");
    //Serial1.print(">" + Incoming_Data + "<");
    //Serial1.flush();
}
