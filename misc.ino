void DHT_read(){
  long now = millis();

  // Reading sensor periodically
  if (now - last_DHT_read_time > DHT_READ_PERIOD) {
    last_DHT_read_time = now;
    
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Humidity validity check
    if ( isnan(h) ) {
      Serial.println("DHT humidity reading error");
    }
    else {
      Serial.print("DHT humidity: ");
      Serial.print(h);
      Serial.println(" %");
      
      DHT_humidity = h;
    }
    
    // Temperature validity check
    if( isnan(t) ){
      Serial.println("DHT temperature reading error");
    }
    else {
      Serial.print("DHT temperature: ");
      Serial.print(t);
      Serial.println(" C, ");
      
      DHT_temperature = t;
    }
  }

  // Send MQTT value periodically
  if( now - last_DHT_publish_time > DHT_PUBLISH_PERIOD) {
    last_DHT_publish_time = now;
    
    // Only send valid temperature
    if(DHT_temperature != 0 && DHT_humidity != 0) {
      
      Serial.println("MQTT publish of DHT data");
      
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();
      root["temperature"] = (String) DHT_temperature;
      root["humidity"] = (String) DHT_humidity;
      root["uptime"] = (String) (now/1000);
      char data[200];
      root.printTo(data, root.measureLength() + 1);
      MQTT_client.publish(MQTT_DHT_STATUS_TOPIC, MQTT_QOS, MQTT_RETAIN, data);
    }
  }
}




void apparent_power_read(){
  long now = millis();

  // Reading sensor periodically
  if (now - last_power_read_time > POWER_READ_PERIOD) {
    last_power_read_time = now;
    double Irms = emon1.calcIrms(1480);  // Calculate Irms only
    apparent_power = MAINS_VOLTAGE*Irms;

    Serial.print("Apparent power: ");
    Serial.print(apparent_power);
    Serial.println(" VA");
  }

  // Send MQTT value periodically
  if( now - last_power_publish_time > POWER_PUBLISH_PERIOD) {
    last_power_publish_time = now;
    
    Serial.println("MQTT publish of apparent power data");
    
    MQTT_client.publish(MQTT_POWER_STATUS_TOPIC, MQTT_QOS, MQTT_RETAIN, String(apparent_power).c_str());
  }
}

