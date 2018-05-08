#include <CurieBLE.h>

BLEPeripheral blePeripheral; // create peripheral instance
BLEService motorService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service


// create switch characteristic and allow remote device to read and write
BLECharCharacteristic motorCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
// create button characteristic and allow remote device to get notifications
//BLECharCharacteristic buttonCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify); // allows remote device to get notifications

void setup() {
  Serial.begin(9600);
  
  blePeripheral.setLocalName("Scooter");
  // set the UUID for the service this peripheral advertises:
  blePeripheral.setAdvertisedServiceUuid(motorService.uuid());
  // add service and characteristics
  blePeripheral.addAttribute(motorService);
  blePeripheral.addAttribute(motorCharacteristic);
  //blePeripheral.addAttribute(buttonCharacteristic);
  // assign event handlers for connected, disconnected to peripheral
  blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
  // advertise the service
  blePeripheral.begin();
  //delay(1000);
  //Serial.println("Bluetooth device active, waiting for connections...");
}

// ====================== Loop ========================
void loop() {
  // poll peripheral
  blePeripheral.poll();

  if (motorCharacteristic.written()) {
    
    // update LED, either central has written to characteristic or button state has changed
    if (motorCharacteristic.value()) {
      Serial.println(motorCharacteristic.value());
    }
  }
}

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

