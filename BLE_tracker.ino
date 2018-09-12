/*
 * BLE PRESENCE DETECTION
 * Maxime MOREILLON
 * 
 * Based on code from Andreas Spiess
 * 
 * Board type: ESP32 dev module
 */

#include "BLEDevice.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include "SSD1306.h"

#include "credentials.h";
//#include "credentials_jtekt_iot.h";

static BLEUUID *pServerUUID;
BLEScan* pBLEScan;
BLEClient*  pClient;

// IO
#define DISPLAY_SDA_PIN 5
#define DISPLAY_SCL_PIN 4

// Display
SSD1306 display(0x3c, DISPLAY_SDA_PIN, DISPLAY_SCL_PIN);
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

bool device_found;
bool device_found_previously = false;

String known_UUIDs[] = {"cdb7950d-73f1-4d4d-8e47-c090502dbd63"};

// MQTT settings
#define MQTT_BROKER_ADDRESS IPAddress(192, 168, 1, 2)
#define MQTT_PORT 1883
#define MQTT_STATUS_TOPIC "location/moreillon"
#define MQTT_PAYLOAD_DETECTED "home"
#define MQTT_PAYLOAD_NOT_DETECTED "not_home"

WiFiClient Wifi_client;
PubSubClient MQTT_client(Wifi_client);

#define TIME_BETWEEN_SUCCESSFULL_SCANS 60000 // [ms]
#define SCAN_DURATION 60 // [s]

long last_successful_scan_time = -TIME_BETWEEN_SUCCESSFULL_SCANS;

void setup() {

  delay(10); // mandatory initial delay

  // Serial init
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");

  MQTT_setup();
  BLE_setup();
  display_setup();
}

void loop() {

  long current_time = millis();
  if(current_time - last_successful_scan_time > TIME_BETWEEN_SUCCESSFULL_SCANS) {
    
    Serial.println();
    Serial.println("BLE Scan restarted");

    display.clear();
    display.drawString(0, 0, "SCANNING...");
    display.display();
  
    device_found = false;
    BLEScanResults scanResults = pBLEScan->start(SCAN_DURATION);
    Serial.println("BLE Scan finished");

    if(device_found != device_found_previously) {
      // Has the scan result changed since last time?
      Serial.println("Scan result different from previously");

      // If so, acknowledge the change
      device_found_previously = device_found;
      
      if (device_found) {
        MQTT_send(MQTT_PAYLOAD_DETECTED);
      }
      else {
        MQTT_send(MQTT_PAYLOAD_NOT_DETECTED);
      }
    }
    else {
      Serial.println("Scan result identical to previously");
    }


    if (device_found) {
      last_successful_scan_time = current_time;
      
      Serial.println("Device has been found during the scan");
      display.clear();
      display.drawString(0, 0, "DEVICE FOUND");
      display.display();
      
    }
    else {
      Serial.println("Device has NOT been found during the scan");
      display.clear();
      display.drawString(0, 0, "DEVICE NOT FOUND");
      display.display();
      
    }
  }
  
} // End of loop
