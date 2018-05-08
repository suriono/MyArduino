#include <CurieBLE.h>

const int ledPin = 13; // set ledPin to on-board LED

BLEPeripheral blePeripheral; // create peripheral instance
// Use an app to find the actual UUID
BLEService bleService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service
BLEIntCharacteristic bleCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite); // read and write
//BLEIntCharacteristic speedCharacteristic("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite); // speed

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
  // central wrote new value to characteristic, update LED
  Serial.print("Characteristic event, written: ");

  if (bleCharacteristic.value()) {
    Serial.print(bleCharacteristic.value());
    Serial.println(" general on");
  } else {
    Serial.println("general off");
  }
  bleCharacteristic.setValue(5);
  //bleCharacteristic.write(blePeripheral, '2', 1);
}

// ========================== Setup =============================

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT); // use the LED on pin 13 as an output
  
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
  // poll peripheral
  blePeripheral.poll();
  
  if(time_last < millis()) {  // using blinking to indicate the status
     if(blePeripheral.connected()) {
        time_last = millis() + 1000;  // blink every second when connected
     } else {
        time_last = millis() + 250;   // else
     }
     led_toggle_state = !led_toggle_state;
     digitalWrite(ledPin, led_toggle_state);
  }

}
