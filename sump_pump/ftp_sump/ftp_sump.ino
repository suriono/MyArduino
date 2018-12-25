#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

char server[] = "myplace.frontier.com";    // name address for Google (using DNS)
EthernetClient client, dclient;
File fh, sumpfile;
char outBuf[128], outCount;
char fileName[] = "SUMP1.js";
#define UltraSonic_Data_Size 20
int ultrasonic_data[UltraSonic_Data_Size], ultrasonic_index=0;


void setup() {
  Serial.begin(9600);
  
  if (!SD.begin(4)) {
    Serial.println("SD card initialization failed!");
  }
  
  byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x98, 0xF3 }; // MAC address
  if (!Ethernet.begin(mac)) {
    Serial.println("Ethernet connection FAIL.");
  }
  delay(1000);
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
}
// *******************************************
void loop() {
  if (Serial.available()) {
    int indepth = Serial.parseInt(); // get integer
  
    add_depth_array(indepth);
    update_website();
  }
}

// ***************** Add to the depth array **************
void add_depth_array(int newint) {
  if (ultrasonic_index >= UltraSonic_Data_Size) { // fill-up
    ultrasonic_index = UltraSonic_Data_Size-1;
    for(int n=0; n < UltraSonic_Data_Size-1; n++) {
      ultrasonic_data[n] = ultrasonic_data[n+1]; 
    }
  }
  ultrasonic_data[ultrasonic_index] = newint;
  
  for(int n=0; n < UltraSonic_Data_Size; n++) {
      Serial.println(ultrasonic_data[n]); 
  }
  ultrasonic_index++;
}

// ***************** Update website **********************
void update_website() {
      SD.remove(fileName);
      sumpfile = SD.open(fileName, FILE_WRITE);
      if(sumpfile) {
         write_data_SD();  // write data in JS format
         fh = SD.open(fileName,FILE_READ);
         if(fh) {
            if(sendFTP()) Serial.println(F("FTP OK"));
            else Serial.println(F("FTP FAIL"));
         } else {
            Serial.print(F("Fail to reopen: "));
            Serial.println(fileName);
            fh.close();   
         }
         fh.close();
      }
}
// ***************** writing SUMP1.js file ***************
void write_data_SD() {
   sumpfile.println("function distance () {");
   sumpfile.print("var dist=[");
   for(int n=0; n < UltraSonic_Data_Size-1; n++) {
      sumpfile.print(ultrasonic_data[n]); 
      sumpfile.write(',');
   }
   sumpfile.print(ultrasonic_data[UltraSonic_Data_Size-1]); 
   sumpfile.println("];"); //sumpfile.flush();
   sumpfile.println("return dist; }");
   sumpfile.close();
}

// ***************** send FTP ****************************
byte sendFTP() {
  if (client.connect(server,21)) {
    Serial.println(F("FTP server is connected"));
  } else {
    //fh.close();
    Serial.println(F("FTP server connection failed"));
    return 0;
  }

  if(!eRcv()) {
    Serial.println("Failed to get response before log on");
    return 0;
  } else {
    Serial.println("Waiting for log on");
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
      Serial.println(F("Bad PASV Answer"));    
    }
  }

  unsigned int hiPort,loPort;
  hiPort = array_pasv[4] << 8;
  loPort = array_pasv[5] & 255;
  Serial.print(F("Data port: "));
  hiPort = hiPort | loPort;
  Serial.println(hiPort);
  if (dclient.connect(server,hiPort)) {
    Serial.println(F("Data connected"));
  } 
  else {
    Serial.println(F("Data connection failed"));
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

  Serial.println(F("Writing"));
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
  Serial.println(F("Data disconnected"));
  if(!eRcv()) return 0;
  client.println(F("QUIT"));
  if(!eRcv()) return 0;
  client.stop();
  Serial.println(F("Command disconnected"));
  fh.close();
  Serial.println(F("SD closed"));
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
    Serial.write(thisByte);
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
    Serial.write(thisByte);
  }
  client.stop();
  Serial.println(F("Command disconnected"));
  fh.close();
  Serial.println(F("SD closed"));
}
// *******************End of doFTP ****************************


