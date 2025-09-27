/*
  Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
  Ported to Arduino ESP32 by Evandro Copercini
  updated by chegewara and MoThunderz
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <FastLED.h>

// Refer to your board, Data pin may be 38 or 48
#define DATA_PIN 38
#define NUM_LEDS 1
CRGB leds[NUM_LEDS];

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
BLECharacteristic* pCharacteristic_2 = NULL;
BLEDescriptor *pDescr;
BLE2902 *pBLE2902;

bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;  // return value to phone
int  recycle_class = 0;  // 0=trash, 1=recycle, 2=unknown

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_2 "e0772cd2-90a9-49fb-a0a1-04fc1cd94e35"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

// Received data ================================
class CharacteristicsCallback: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pChar) override {
   // std::string pChar2_value_stdstr = std::string(pChar->getValue());
    String pChar2_value_stdstr = pChar->getValue();
    String pChar2_value_string = String(pChar2_value_stdstr.c_str());
    recycle_class = pChar2_value_stdstr.toInt();
   // switch (pChar2_value_int) {
    //  case 1:
    //    Serial.println("Close");
    //    break;
    //  case 2:
    //    Serial.println("Open");
    //    break;
    //  case 3:
    //    Serial.println("Trash");
    //    break;
    //  default:
    //    Serial.println("Unknown");
    //    break;
    // }
    //Serial.println("pChar2: " + String(recycle_class));
  }
};

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); // GRB ordering is assumed
  // Create the BLE Device
  BLEDevice::init("Recycler");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );                   
 pCharacteristic_2 = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_2,
                      BLECharacteristic::PROPERTY_READ | 
                      BLECharacteristic::PROPERTY_WRITE
                    );                   

  // Create a BLE Descriptor
  
  pDescr = new BLEDescriptor((uint16_t)0x2901);
  pDescr->setValue("A very interesting variable");
  pCharacteristic->addDescriptor(pDescr);
  
  pBLE2902 = new BLE2902();
  pBLE2902->setNotifications(true);
  pCharacteristic->addDescriptor(pBLE2902);
  
  pCharacteristic_2->addDescriptor(new BLE2902());
  pCharacteristic_2->setCallbacks(new CharacteristicsCallback()); 

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
    static bool isConnected_not_previousy = false;
    static unsigned long last_notify = millis();
    static int last_recycle_class = -1;
    // notify changed value
    if (deviceConnected) {                   // if connected
      //  if ( (millis() - last_notify) > 1000) {
            pCharacteristic->setValue(value);    // send back value
            pCharacteristic->notify();
            value++;
            last_notify = millis();
     //   }
      //  leds[0] = CRGB::Blue;
        if (isConnected_not_previousy) {
            leds[0] = CHSV(204,255,100);
            FastLED.show();
            Serial.println("Device is connected");
            isConnected_not_previousy = false;
        }

        if ( recycle_class != last_recycle_class) {
          
            last_recycle_class = recycle_class;

            switch (recycle_class) {
                case 1:
                    Serial.println("Trash");
                    leds[0] = CHSV(8,255,100);
                    break;
                case 2:
                    leds[0] = CHSV(90,255,100);
                    Serial.println("Recycle");
                    break;
                default:
                    leds[0] = CHSV(165,255,100);
                    Serial.println("Unknown");
                    break;
            }
            FastLED.show();
            Serial.println("Run selection: " + String(recycle_class));
        }

        delay(1000);
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) { // false & true, previously connected
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
        leds[0] = CHSV(15,255,100);
       // leds[0] = CRGB::Red;
        FastLED.show();
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) { // true & false, previously not connected
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
        isConnected_not_previousy = true;
    }
}
