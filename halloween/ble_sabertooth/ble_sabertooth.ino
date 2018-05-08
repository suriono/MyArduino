#include <SoftwareSerial.h>
#include <SabertoothSimplified.h>
#include <CurieBLE.h>

#define NOT_A_PIN 0
SoftwareSerial SWSerial(NOT_A_PIN, 11); // RX on no pin (unused), TX on pin 11 (to S1).
SabertoothSimplified ST(SWSerial); // Use SWSerial as the serial port.
//int motor_Offset = 0;        // motor offset when goes in straight line
#define LEFT  false
#define RIGHT true

const int ledPin = 13; // set ledPin to on-board LED
unsigned long last_cmd_time = millis();

BLEPeripheral blePeripheral; // create peripheral instance
// Use an app to find the actual UUID
BLEService bleService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service
BLEIntCharacteristic bleCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify); // read and write
//BLEIntCharacteristic speedCharacteristic("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify); ; // speed

// ======================= BLE Event Handlers =====================
void blePeripheralConnectHandler(BLECentral& central) {
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}
void blePeripheralDisconnectHandler(BLECentral& central) {
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}
void CharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  Serial.print("BLE: ");
  
  if (bleCharacteristic.value()) {
    Serial.print(bleCharacteristic.value());
    int motor_pow = bleCharacteristic.value() & 127; // first 7 bits are power 0-127
    int motor_degree = 3*(bleCharacteristic.value() >> 7); // last 7 bits are angle in 3 degree increment
    Serial.print(" Power: "); Serial.print(motor_pow);
    Serial.print(" Angle: "); Serial.println(motor_degree);
    Motor_Run(motor_pow, motor_degree);

    last_cmd_time = millis() + 1000;   // 1 secs of new command of motor will stop
  //} else if(speedCharacteristic.value()) {
  //  Serial.print("Speed: ");
  //  Serial.println(speedCharacteristic.value());
  } else {
    Serial.println("Other input");
  }
 
}

// ========================== Setup =============================

void setup() {
  SWSerial.begin(38400);  // Sabertooth software serial
  SWSerial.flush();
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT); // use the LED on pin 13 as an output

  ST.stop();               // stop motors
  
  // set the local name peripheral advertises
  blePeripheral.setLocalName("Scooter");
  // set the UUID for the service this peripheral advertises:
  blePeripheral.setAdvertisedServiceUuid(bleService.uuid());
  // add service and characteristics
  blePeripheral.addAttribute(bleService);
  blePeripheral.addAttribute(bleCharacteristic);
  //blePeripheral.addAttribute(speedCharacteristic);
  // assign event handlers for connected, disconnected to peripheral
  blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
  bleCharacteristic.setEventHandler(BLEWritten, CharacteristicWritten);
  bleCharacteristic.setValue(0);
  // advertise the service
  blePeripheral.begin();
  delay(5000);
  Serial.println("Bluetooth device active, waiting for connections...");
}

// =========================== Loop ==============================
unsigned long time_last=millis();
void loop() {
  static boolean led_toggle_state;
  static int nn=0;
  
  // poll peripheral
  blePeripheral.poll();
  
  if(time_last < millis()) {  // using blinking to indicate the status
     if(blePeripheral.connected()) {
        led_toggle_state = false;
        time_last = millis() + 5000;  // blink every second when connected
        //bleCharacteristic.setValue(nn++);
        //speedCharacteristic.setValue(nn++);
     } else {
        time_last = millis() + 1000;   // else
        ST.stop();   // stop the motor when disconnected
        //Serial.print("Motor Stop");
     }
     led_toggle_state = !led_toggle_state;
     digitalWrite(ledPin, led_toggle_state);
  }

  if (millis() > last_cmd_time) {
     ST.stop();    // has not received new command, stop for safety
     //Serial.println("Motor Stop");
  }
}

// ========================= Motor run general ========================
void Motor_Run(byte pow_mot, int degree) { // (0 to 127, 0 to 360)
  float x = cos(degree*PI/180.0);
  float y = sin(degree*PI/180.0);
  float norm_f = abs(pow_mot) / max(abs(y-x),abs(y+x));
  int motor1 = round((y-x) * norm_f);
  int motor2 = round(-(y+x) * norm_f);
  
  Serial.print("Power="); Serial.print(pow_mot);
  Serial.print("  Degree="); Serial.print(degree);
  Serial.print("  M1="); Serial.print(motor1); 
  Serial.print(",M2=");  Serial.println(motor2);
  
  ST.motor(1, motor1); delay(10);
  ST.motor(2, motor2); delay(10);
  ST.motor(1, motor1); delay(10);
  ST.motor(2, motor2); delay(10);
}
