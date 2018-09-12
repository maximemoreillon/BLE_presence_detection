void MQTT_setup() {
  MQTT_client.setServer(MQTT_BROKER_ADDRESS, MQTT_PORT);
  MQTT_client.setCallback(MQTT_message_callback);
}

void MQTT_connect() {

  while (!MQTT_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (MQTT_client.connect("", MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("connected");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(MQTT_client.state());
      Serial.println(" Trying again...");
      // Wait 1 second before retrying
      delay(1000);
    }
  }
  
}


void MQTT_message_callback(char* topic, byte* payload, unsigned int len) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < len; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // No action om message
}


void MQTT_send(char* payload) {

  // Connect to Wifi
  display.clear();
  display.drawString(0, 0, "CONNECTING TO WIFI...");
  display.display();
  wifi_setup_v3();

  // Connect MQTT
  display.clear();
  display.drawString(0, 0, "CONNECTING TO MQTT...");
  display.display();
  MQTT_connect();

  // Send payload
  Serial.println("Sending MQTT payload");
  display.clear();
  display.drawString(0, 0, "SENDING MQTT PAYLOAD...");
  display.display();
  MQTT_client.publish(MQTT_STATUS_TOPIC, payload);

  // Disconnnect MQTT
  Serial.println("Disconnecting MQTT");
  display.clear();
  display.drawString(0, 0, "DISCONNECTING MQTT...");
  display.display();
  MQTT_client.disconnect();


  // Disconnect Wifi
  Serial.println("Disconnecting Wifi");
  display.clear();
  display.drawString(0, 0, "DISCONNECTING WIFI...");
  display.display();
  WiFi.mode(WIFI_OFF);

  Serial.println("Done sending MQTT payload");
    
}

