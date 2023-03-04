#include "EasyNextionLibrary.h"  // Include EasyNextionLibrary

SoftwareSerial myserial(4,5);  // you have to change the library code HardWareSerial -> SoftwareSerial, and SoftSerial -> SoftwareSerial
                               // also add #include <SoftwareSerial.h> in the library
EasyNex myNex(myserial);       // Create an object of EasyNex class with the name < myNex >
                               // Set as parameter the Hardware Serial you are going to use
//EasyNex myNex(Serial); // Create an object of EasyNex class with the name < myNex >
                       // Set as parameter the Hardware Serial you are going to use
int maxSpeed = 0;

void setup(){
  myNex.begin(9600); // Begin the object with a baud rate of 9600
  Serial.begin(57600);  
                     // If no parameter was given in the begin(), the default baud rate of 9600 will be used
  
  pinMode(LED_BUILTIN, OUTPUT); // The built-in LED is initialized as an output
  digitalWrite(LED_BUILTIN, LOW);
}

void loop(){
  static unsigned long lasttime;
  myNex.NextionListen(); // WARNING: This function must be called repeatedly to response touch events
                         // from Nextion touch panel. Actually, you should place it in your loop function.
  if ( (millis()-lasttime)> 1000) {
    lasttime = millis();
    maxSpeed = myNex.readNumber("h0.val");
    Serial.print("Max Speed:"); Serial.println(maxSpeed);
    myNex.writeNum("n0.val", maxSpeed);
  }
}

void trigger0(){
  // To call this void send from Nextion's component's Event:  printh 23 02 54 00
  // In this exmaple, we send this command from the Release Event of b0 button (see the HMI of this example)
  // You can send  the same `printh` command, to call the same function, from more than one component, depending on your needs
  Serial.println("Trigger0");

  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // If LED_BUILTIN is ON, turn it OFF, or the opposite
  if(digitalRead(LED_BUILTIN) == HIGH){
    myNex.writeNum("b0.bco", 2016); // Set button b0 background color to GREEN (color code: 2016)
    myNex.writeStr("b0.txt", "ON"); // Set button b0 text to "ON"
    myNex.writeNum("p0.pic", 1);    // Set picture 1 as background picture for p0
    
  }else if(digitalRead(LED_BUILTIN) == LOW){
    myNex.writeNum("b0.bco", 63488); // Set button b0 background color to RED (color code: 63488)
    myNex.writeStr("b0.txt", "OFF"); // Set button b0 text to "OFF"
    myNex.writeNum("p0.pic", 0);     // Set picture 0 as background picture for p0 picture component
  }
  //myNex.writeNum("h0.val", 25);
  
}

// =============================================================================================

void trigger1(){
  Serial.println("Trigger1");
  // To call this void send from Nextion's component's Event:  printh 23 02 54 01
  // In this exmaple, we send this command from the Release Event of b1 button (see the HMI of this example)
  // You can send  the same `printh` command, to call the same function, from more than one component, depending on your needs
  
  if(digitalRead(LED_BUILTIN) == HIGH){
    digitalWrite(LED_BUILTIN, LOW);  // Start the function with the LED off
    myNex.writeNum("b0.bco", 63488); // Set button b0 background color to RED (color code: 63488)
    myNex.writeStr("b0.txt", "OFF"); // Set button b0 text to "OFF"
    myNex.writeNum("p0.pic", 0);     // Set picture 0 as background picture for p0 picture component
  }
  //myNex.writeNum("h0.val", 80);
  //myNex.writeStr("t0.txt", "LED STROBE\\rON");                                              
}

// =============================================================================================

void trigger2(){
 // Serial.print("Trigger2: "); Serial.println(myNex.readNumber("h0.val"));
  
  
}