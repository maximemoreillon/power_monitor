void MQTT_setup(){

  // Callbacks
  MQTT_client.onConnect(MQTT_connect_callback);
  MQTT_client.onDisconnect(MQTT_disconnect_callback);
  MQTT_client.onSubscribe(MQTT_subscribe_callback);
  MQTT_client.onUnsubscribe(MQTT_unsubscribe_callback);
  MQTT_client.onMessage(MQTT_message_callback);
  MQTT_client.onPublish(MQTT_publish_callback);

  // Settings
  MQTT_client.setServer(MQTT_BROKER_ADDRESS, MQTT_PORT);
  MQTT_client.setCredentials(MQTT_USERNAME, MQTT_PASSWORD);
}

void MQTT_connect() {
  Serial.println("MQTT connecting...");

  MQTT_reconnect_timer.detach();
  MQTT_client.connect();
}

void MQTT_connect_callback(bool sessionPresent) {
  Serial.println("MQTT connected");
}

void MQTT_disconnect_callback(AsyncMqttClientDisconnectReason reason) {
  Serial.println("MQTT disconnected");

  if (WiFi.isConnected()) {
    MQTT_reconnect_timer.attach(2, MQTT_connect);
  }
}

void MQTT_subscribe_callback(uint16_t packetId, uint8_t qos) {
  Serial.println("MQTT subscribed");
}

void MQTT_unsubscribe_callback(uint16_t packetId) {
  Serial.println("MQTT unsubscribed");
}

void MQTT_message_callback(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {

  Serial.print("MQTTT message received: ");
  Serial.print("  topic: ");
  Serial.print(topic);
  Serial.print("  payload: ");
  Serial.print(payload);
  Serial.print("  qos: ");
  Serial.print(properties.qos);
  Serial.print("  dup: ");
  Serial.print(properties.dup);
  Serial.print("  retain: ");
  Serial.print(properties.retain);
  Serial.print("  len: ");
  Serial.print(len);
  Serial.print("  index: ");
  Serial.print(index);
  Serial.print("  total: ");
  Serial.print(total);
  Serial.println("");

}

void MQTT_publish_callback(uint16_t packetId) {
  Serial.println("MQTT published");
}

