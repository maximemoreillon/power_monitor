/*
 * POWER MONITOR
 * Maxime MOREILLON
 * 
 * Board type: Wemos D1 Mini
 * 
 */

// Libraries
#include <ESP8266WiFi.h> // Main ESP8266 library
#include <ArduinoOTA.h> // OTA update library
#include <WiFiUdp.h> // Required for OTA
#include <AsyncMqttClient.h> // MQTT library
#include <Ticker.h> // Used when reconecting MQTT upon wifi drop
#include <DHT.h>
#include "EmonLib.h" // Energy monitor
#include <ArduinoJson.h>

#include "credentials.h";


// Wifi
WiFiEventHandler wifi_connect_handler;
WiFiEventHandler wifi_disconnect_handler;
Ticker wifi_reconnect_timer;
#define HOSTNAME "powerMonitor"

// MQTT
AsyncMqttClient MQTT_client;
Ticker MQTT_reconnect_timer;
#define MQTT_BROKER_ADDRESS IPAddress(192, 168, 1, 2)
#define MQTT_PORT 1883
#define MQTT_POWER_STATUS_TOPIC "power/status"
#define MQTT_DHT_STATUS_TOPIC "dht/bathroom/status"
#define MQTT_QOS 1
#define MQTT_RETAIN true

// Pin mapping
#define CT_PIN A0
#define DHT_PIN D2

// Power monitor
EnergyMonitor emon1;
#define POWER_READ_PERIOD 30000 // [ms] = 30 seconds
#define POWER_PUBLISH_PERIOD 120000 // [ms] = 2 minutes
#define MAINS_VOLTAGE 100.00 // [V]
float apparent_power;
long last_power_read_time, last_power_publish_time;


// DHT 
DHT dht(DHT_PIN, DHT22);
#define DHT_PUBLISH_PERIOD 300000 // [ms], 300000 is 5 minutes
#define DHT_READ_PERIOD 10000 // [ms]
long last_DHT_publish_time, last_DHT_read_time;
float DHT_temperature = 0;
float DHT_humidity = 0;


void setup() {
  // Mandatory initial delay
  delay(10);

  // Serial init
  Serial.begin(115200);
  Serial.println(); // Separate serial stream from initial gibberish
  Serial.println(F(__FILE__ " " __DATE__ " " __TIME__));

  emon1.current(CT_PIN, 111.1); // Current: input pin, calibration.
  dht.begin();
  wifi_setup();
  MQTT_setup();
  OTA_setup();

  wifi_connect();
}

void loop() {
  ArduinoOTA.handle();
  DHT_read();
  apparent_power_read();
}
