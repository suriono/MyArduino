#define BUFFERSIZE 16
uint8_t buf[BUFFERSIZE];
int num = 0;
int counter = 0;
int incr_buff_size = BUFFERSIZE;

int incomingByte = 0;

boolean incoming_wifi() {  
   wificlient = server.available();  
  // client.setTimeout(2); 
   boolean isValidCommand = false;  

  // if (client.connected()) {

      if ( wificlient ) {
      
         incomingByte = wificlient.read(); // wificlient.read();
         while (incomingByte < 0 ) {
            incomingByte = wificlient.read(); // wificlient.read();
         }
         while (incomingByte > -1) {
             buf[num] = uint8_t(incomingByte);
             num++;
             if (num == BUFFERSIZE) {
                Serial1.write(buf,num);
                //  Serial1.write(incomingByte);
                Serial1.flush();
                // Serial.println(incomingByte,DEC);
                Serial.println(buf[0]);
                num = 0;
             }
             incomingByte = wificlient.read(); // wificlient.read();
         }
      }
       isValidCommand = true;
   //} 
  return isValidCommand;
}
