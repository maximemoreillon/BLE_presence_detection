static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic,  uint8_t* pData,  size_t length,  bool isNotify) {
  // No idea what this does
  Serial.print("Notify callback for characteristic ");
  Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
  Serial.print(" of data length ");
  Serial.println(length);
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    /**
        Called for each advertising BLE server.
    */
    
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.print("BLE Advertised Device found: ");
      Serial.println(advertisedDevice.toString().c_str());
      
      // Check if the BLE device is advertising its UUID
      if(advertisedDevice.haveServiceUUID()) {

        // If so, get the UUID
        pServerUUID = new BLEUUID(advertisedDevice.getServiceUUID());
        
        Serial.println( "Device has a UUID");
        
        // Compare it with the UUID we're looking for
        
        for (int i = 0; i < (sizeof(known_UUIDs) / sizeof(known_UUIDs[0])); i++) {
          if (strcmp(pServerUUID->toString().c_str(), known_UUIDs[i].c_str()) == 0) {

            Serial.println("Device with a matching UUID found");
            device_found = true;

            // No need to continue scanning
            Serial.println("Stopping BLE scan");
            advertisedDevice.getScan()->stop();
          }
        }
      }
    }
}; // MyAdvertisedDeviceCallbacks

void BLE_setup() {
  BLEDevice::init("");

  pClient  = BLEDevice::createClient();
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
}
