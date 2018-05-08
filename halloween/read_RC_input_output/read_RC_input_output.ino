int Read_RefPin = A0; // Analog input 0 to 1023 for Reference Center, green cable
int Read_LRPin = A1;   // Analog input of Left/Right, yellow cable
int Read_FBPin = A2;   // Analog input of Front/Back, white cable
int Out_Ref = 9;  // Analog PWM 0 to 255 for reference
int Out_LR = 10;  // Analog PWM 0 to 255 for reference
int Out_FB = 11;   // Analog PWM for Left/Right
int readvalue, Ref_outvalue, LR_outvalue, FB_outvalue;
int incr = 0;

void setup() {
  Serial.begin(9600);
  //pinMode(Out_Pin, OUTPUT);
  Serial.println("Enter a number, 0 to 5V");
  Ref_outvalue = 130;
  LR_outvalue = 128;
  FB_outvalue = 128;
  for (int nn=0; nn<10; nn++) {
     analogWrite(Out_Ref, Ref_outvalue);
     analogWrite(Out_LR, LR_outvalue);
     analogWrite(Out_FB, FB_outvalue);
     Serial.println(Ref_outvalue); // 0 to 255
     Serial.println(LR_outvalue);
     Serial.println(FB_outvalue);
     delay(2000);
     
     readvalue = analogRead(Read_RefPin);
     Serial.println(readvalue);  
     if (readvalue < 512) {
       Ref_outvalue++;
     } else if (readvalue > 512) {
       Ref_outvalue--;
    }
    readvalue = analogRead(Read_LRPin);
    Serial.println(readvalue);  
    if (readvalue < 512) {
       LR_outvalue++;
    } else if (readvalue > 512) {
       LR_outvalue--;
    }
    readvalue = analogRead(Read_FBPin);
    Serial.println(readvalue);  
    Serial.println("============");
    if (readvalue < 512) {
       FB_outvalue++;
    } else if (readvalue > 512) {
       FB_outvalue--;
    }
  }
}

void loop() {
    readvalue = analogRead(Read_RefPin);
    Serial.print("Ref out:" + String(Ref_outvalue) + ",");
    Serial.println(readvalue*5.0/1023.0); // in Volt
    
    readvalue = analogRead(Read_LRPin);
    Serial.print("L/R out:" + String(LR_outvalue) + ",");
    Serial.println(readvalue*5.0/1023.0); // in Volt
    
    readvalue = analogRead(Read_FBPin);
    Serial.print("F/B out:" + String(FB_outvalue) + ",");
    Serial.println(readvalue*5.0/1023.0); // in Volt
    
    if (incr < 1) {
       analogWrite(Out_LR, 57);
       delay(5000);
    } else {
       analogWrite(Out_LR, LR_outvalue);
       delay(5000);
    }
    incr = 1;
    
    Serial.println("====");
    delay(10000);
}

