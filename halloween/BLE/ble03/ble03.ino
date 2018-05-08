/*
  Copyright (c) 2015 Intel Corporation. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-
  1301 USA
*/


#include <CurieBLE.h>

const int ledPin = 13; // set ledPin to on-board LED
const int buttonPin = 4; // set buttonPin to digital pin 4

BLEPeripheral blePeripheral; // create peripheral instance
BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service


// create switch characteristic and allow remote device to read and write
BLECharCharacteristic ledCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
//BLEStrCharacteristic ledCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
// create button characteristic and allow remote device to get notifications
//BLECharCharacteristic buttonCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify); // allows remote device to get notifications

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
void switchCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  // central wrote new value to characteristic, update LED
  Serial.print("Characteristic event, written: ");

  if (ledCharacteristic.value()) {
    Serial.print(ledCharacteristic.value());
    Serial.println(",LED on");
    digitalWrite(ledPin, HIGH);
  } else {
    Serial.println("LED off");
    digitalWrite(ledPin, LOW);
  }
}

// ========================== Setup =============================

void setup() {
  Serial.begin(9600);
  //pinMode(ledPin, OUTPUT); // use the LED on pin 13 as an output
  //pinMode(buttonPin, INPUT); // use button pin 4 as an input

  // set the local name peripheral advertises
  blePeripheral.setLocalName("ButtonLED");
  // set the UUID for the service this peripheral advertises:
  blePeripheral.setAdvertisedServiceUuid(ledService.uuid());

  // add service and characteristics
  blePeripheral.addAttribute(ledService);
  blePeripheral.addAttribute(ledCharacteristic);
  //blePeripheral.addAttribute(buttonCharacteristic);
  // assign event handlers for connected, disconnected to peripheral
  blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
  ledCharacteristic.setEventHandler(BLEWritten, switchCharacteristicWritten);
  //ledCharacteristic.setValue(0);
 
  // advertise the service
  blePeripheral.begin();
  //delay(1000);
  //Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // poll peripheral
  blePeripheral.poll();
/*
  if (ledCharacteristic.written() ) { //  || buttonChanged) {
    // update LED, either central has written to characteristic or button state has changed
    if (ledCharacteristic.value()) {
      Serial.print("Read = ");
      Serial.println(ledCharacteristic.value());
      Serial.println("LED on");
      digitalWrite(ledPin, HIGH);
    } else {
      Serial.println("LED off");
      digitalWrite(ledPin, LOW);
    }
  }
  */
}
