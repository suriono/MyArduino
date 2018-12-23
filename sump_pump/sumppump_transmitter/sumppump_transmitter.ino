#include <NewPing.h>
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

#define server "myplace.frontier.com"    // name address for Google (using DNS)
EthernetClient client, dclient;
File fh;
char outBuf[128], outCount;
#define fileName "SUMP1.js"              // SD filename contains array of data
#define MAX_DISTANCE 50 // Maximum ultra sonic centimeters (rated 400-500cm)

NewPing Sonar(9, 8, MAX_DISTANCE); // trigger,echo,max distance

void setup() {
  Serial.begin (57600);
  //Serial.setTimeout(5000);
  //if (!SD.begin(4)) {
  SD.begin(4);
    //Serial.println("SD card initialization failed!");
  //  return;
  //}
  
  byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x98, 0xF3 }; // MAC address
  //if (!Ethernet.begin(mac)) {
  Ethernet.begin(mac);
    //Serial.println("Ethernet connection FAIL.");
  //}
  //Serial.print("My IP address: ");
  //Serial.println(Ethernet.localIP());
}

void loop() {
  delay(100); 
}

// ****************** Reading Serial ********************
void serialEvent() {
  String readstr;
  String tmpstr = Serial.readStringUntil('>');
 
  //while (tmpstr.length()>0) { // in case any leftover older input
  //  tmpstr = Serial.readStringUntil('>');
  //}
  readstr = Serial.readStringUntil('<'); //13=carriage return
  while(Serial.available()) { // read any left over buffer input
     char cc = Serial.read();
  }
  
  if(readstr == "Send_distance") {
    String outstr = ">D=" + String(Sonar_distance()) + "<";
    Serial.print(outstr);
    //Serial.flush();
  } else if(readstr.indexOf("Web_header=") == 0) { // post to website
      SD.remove(fileName);
      fh = SD.open(fileName, FILE_WRITE);
      if(fh) {
         fh.print(readstr.substring(readstr.indexOf("=")+1)) ;
         //fh.flush();
      //} else {
      //   Serial.print(">Fail:SD<");
      }
  } else if(readstr.indexOf("Data=") == 0) { // data stream
      fh.print(readstr.substring(readstr.indexOf("=")+1)) ; 
      //fh.flush();
  //} else if(readstr.indexOf("Close_file") == 0) { // data stream
  //    fh.close();
  } else if(readstr.indexOf("Web_update") == 0) { // update web
      //fh.close();
      update_website();
  } else if(readstr.indexOf("SD_comma") == 0) { // retrieve SD data
      fh.close();
      fh = SD.open(fileName, FILE_READ);
      //Serial.print('>'); Serial.flush(); delay(500);
      int ncomma = 0;
      while (fh.available()) {
        //Serial.print(char(fh.read()));
        if (char(fh.read()) == ',') {
           ncomma++;
        }
        //delay(40);
      }
      //Serial.print('<'); Serial.flush();
      Serial.print(">Comma=" + String(ncomma) + "<");
      fh.close();
  }
}

//************************** Sonar routines *********************
int Sonar_distance() {
  //byte dist;
  //byte n=0;
  //do {
  //  dist = byte(Sonar.ping_median() / US_ROUNDTRIP_CM);
  //  delay(100);
  //  n++;
  //} while (dist < 1 && n < 4);
  //return dist; 
  return int(Sonar.ping_median() / US_ROUNDTRIP_CM);
}

// ***************** Update website **********************
void update_website() {
   fh = SD.open(fileName,FILE_READ);
   if(fh) {
      //client.connect(server,21);
      sendFTP();
            //if(sendFTP()); // Serial.println(F("FTP OK"));
            //else Serial.println(F("FTP FAIL")); 
      fh.close();
 //  } else {
 //           Serial.print(">Fail: SUMP1<"); Serial.flush();
   } 
}

// ***************** send FTP ****************************
byte sendFTP() {
  if (client.connect(server,21)) {
    //Serial.println(F("FTP server is connected"));
  } else {
    //fh.close();
    //Serial.println(F("FTP server connection failed"));
    return 0;
  }

  if(!eRcv()) {
    //Serial.println("Failed to get response before log on");
    return 0;
  //} else {
  //  Serial.println("Waiting for log on");
  }
  client.println(F("USER usmansuriono")); // enter username
  if(!eRcv()) return 0;
    //else Serial.println("Success entering username");
  client.println(F("PASS yongkain88"));  // enter password
  if(!eRcv()) return 0;
    //else Serial.println("Success entering password");
  
  client.println(F("SYST"));
  if(!eRcv()) return 0;
  client.println(F("PASV"));
  if(!eRcv()) return 0;
  
  client.println("CWD public_html/sump"); // go to FTP public_html folder

  char *tStr = strtok(outBuf,"(,");
  int array_pasv[6];
  for ( int i = 0; i < 6; i++) {
    tStr = strtok(NULL,"(,");
    array_pasv[i] = atoi(tStr);
    if(tStr == NULL) {
      //Serial.println(F("Bad PASV Answer"));    
    }
  }

  unsigned int hiPort,loPort;
  hiPort = array_pasv[4] << 8;
  loPort = array_pasv[5] & 255;
  //Serial.print(F("Data port: "));
  hiPort = hiPort | loPort;
  //Serial.println(hiPort);
  if (dclient.connect(server,hiPort)) {
    //Serial.println(F("Data connected"));
  } 
  else {
    //Serial.println(F("Data connection failed"));
    client.stop();
    fh.close();
    return 0;
  }
  
  client.print(F("STOR "));
  client.println(fileName);
  if(!eRcv()) {
    dclient.stop();
    return 0;
  }

  //Serial.println(F("Writing"));
  byte clientBuf[64];
  int clientCount = 0;
  while(fh.available()) {
    clientBuf[clientCount] = fh.read();
    clientCount++;
    if(clientCount > 63) {
      dclient.write(clientBuf,64);
      clientCount = 0;
    }
  }
  if(clientCount > 0) dclient.write(clientBuf,clientCount);
  dclient.stop();
  //Serial.println(F("Data disconnected"));
  if(!eRcv()) return 0;
  client.println(F("QUIT"));
  if(!eRcv()) return 0;
  client.stop();
  //Serial.println(F("Command disconnected"));
  fh.close();
  //Serial.println(F("SD closed"));
  return 1;
}

byte eRcv() {
  byte respCode;
  byte thisByte;
  while(!client.available()) delay(1);
  respCode = client.peek();
  outCount = 0;
  while(client.available()) {  
    thisByte = client.read();    
    //Serial.write(thisByte);
    if(outCount < 127) {
      outBuf[outCount] = thisByte;
      outCount++;      
      outBuf[outCount] = 0;
    }
  }

  if(respCode >= '4'){
    efail();
    return 0;  
  }
  return 1;
}

void efail(){
  byte thisByte = 0;
  client.println(F("QUIT"));
  while(!client.available()) delay(1);
  while(client.available()) {  
    thisByte = client.read();    
    //Serial.write(thisByte);
  }
  client.stop();
  //Serial.println(F("Command disconnected"));
  fh.close();
  //Serial.println(F("SD closed"));
}
// *******************End of doFTP ****************************

