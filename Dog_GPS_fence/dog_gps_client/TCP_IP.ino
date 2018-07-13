

// ============= Write to Client =================
void write_client() {
  // send a standard http response header
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");  // the connection will be closed after completion of the response
  //client.println("Refresh: 10");  // refresh the page automatically every 5 sec
  client.println();
  //client.println("<!DOCTYPE HTML>");
  //client.println("<html>");

  //unsigned long sendtime = millis();

  client.print("LT ");
  client.print(last_lat,8);
  client.print(",LN ");
  client.print(last_lon,8);
  client.print(",TM ");
  client.print(last_time);

  //client.println("<br/></html>");
}

