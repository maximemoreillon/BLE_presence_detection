void wifi_setup() {

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Wifi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Wifi connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void wifi_setup_v3() {
  // Attempt connection to wifi for a given time
  // If connection unsuccessful, reset Wifi and try again
  long wifi_connect_timeout = 5000; // [ms]
  
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
    Serial.print("Wifi connecting");
    long connect_start_time = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - connect_start_time < wifi_connect_timeout) {
      delay(500);
      Serial.print(".");
    }
    
    Serial.println();
    
    if(WiFi.status() == WL_CONNECTED) {
      Serial.print("Wifi connected, IP address: ");
      Serial.println(WiFi.localIP());
    }
    else {
      Serial.println("Wifi connection timeout");
      WiFi.mode(WIFI_OFF);
    }
  }
}

