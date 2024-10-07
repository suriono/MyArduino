#include <SoftwareSerial.h>
//SoftwareSerial EEBlue(10, 11); // RX=D5 | TX=D6
SoftwareSerial EEBlue(14, 12); // RX=D5=14 | TX=D6=12

void setup()
{
  Serial.begin(57600);
  delay(1000);
  EEBlue.begin(38400);  //Baud Rate for command Mode. 
  delay(1000);
  Serial.println("Enter AT commands!");
}
 
void loop()
{
 
  // Feed any data from bluetooth to Terminal.
  if (EEBlue.available())
    Serial.write(EEBlue.read());
 
  // Feed all data from termial to bluetooth
  if (Serial.available())
    EEBlue.write(Serial.read());

  //delay(1000);
  //Serial.println("hello");
}
