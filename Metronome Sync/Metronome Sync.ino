/*  Web Server
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 */
#include <SPI.h>
#include <Ethernet.h>
#define SERIAL_ENABLE true

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x98, 0xF3 }; // MAC address
//IPAddress ip(192, 168, 1, 177);  // for fixed IP
EthernetServer server(80); // port 80 is default HTTP
EthernetClient client;
boolean currentLineIsBlank = true;

void setup() {
  //pinMode(4, OUTPUT);  // These two lines if SD card is inserted
  //digitalWrite(4, HIGH);  // but SD card is not used
  Serial.begin(9600);
  //Ethernet.begin(mac, ip);
  Ethernet.begin(mac);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}



// ================ Loop ===========================
int counter = 0;
byte count_newline = 0;
char readchar;
String readstr;
void loop() {
  // listen for incoming clients
  client = server.available();
  if (client) {
    serial_println("=========="); serial_println("New client");
    // an http request ends with a blank line
    currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        readchar = client.read();
        readstr += readchar;
        //serial_write(c);
        if (readchar == '\n') {
           //serial_println(readstr);  // print other inputs
           readstr = "";
        } else if (readstr.compareTo("tempo=") == 0) {
           serial_println("found tempo");
           //serial_println(readstr);
           readstr = "";
        }
        //serial_println(String(count_newline));
        //   break;
        //} else {
        //serial_write(readchar);
        //}
      } else {
        break;
      }
    }
    // give the web browser time to receive the data
    String outstr = "Tempo=" + readstr;
    write_all_clients(outstr);
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

// =============== Print Serial ===================
void serial_println(String str) {
   if(SERIAL_ENABLE) {
      Serial.println(str); 
   }
}
void serial_write(char c) {
   if(SERIAL_ENABLE) {
      Serial.write(c); 
   }
}

// ============= Write to Client =================
void write_client(String outstr) {
    // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          //client.println("Refresh: 10");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
         
          client.print(outstr);
          
          client.println("<br/></html>");
}
// ============= Write to all clients =================
void write_all_clients(String outstr) {
    // send a standard http response header
          server.println("HTTP/1.1 200 OK");
          server.println("Content-Type: text/html");
          //server.println("Connection: close");  // the connection will be closed after completion of the response
          server.println();
          server.println("Connected to Server. Wait for trigger");
          //server.println(client.localIP());
          //server.println("Delay 2 seconds");
          //delay(2000);
          
          //server.println("HTTP/1.1 200 OK");
          //server.println("Content-Type: text/html");
          //server.println("Connection: close");  // the connection will be closed after completion of the response
          //server.println();
          server.print(outstr);
}

