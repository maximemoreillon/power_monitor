/*
 * POWER MONITOR
 * Maxime MOREILLON
 * 
 * Board type: ESP32 DEv Board
 * 
 */

#include "IotKernel.h"
#include "RmsCurrentMeasurement.cpp"
#include <WebSocketsServer.h>


// Pin mapping
#define CT_1_PIN 36
#define CT_2_PIN 39

// MQTT
#define MQTT_POWER_STATUS_TOPIC "power/status"

// WS
#define WS_PORT 8087

// Timing
#define MQTT_PUBLISH_PERIOD 10000
#define WS_SEND_PERIOD 500


IotKernel iot_kernel("power","0.1.1"); 

RmsCurrentMeasurement phase_1_measurement;
RmsCurrentMeasurement phase_2_measurement;

WebSocketsServer ws_server = WebSocketsServer(WS_PORT);


void setup() {
  iot_kernel.init();
  mqtt_config();
  ws_setup();
}

void loop() {

  iot_kernel.loop();
  ws_server.loop();


  phase_1_measurement.sample(CT_1_PIN);
  phase_2_measurement.sample(CT_2_PIN); 
  
  periodic_MQTT_publish();
  periodic_ws_broadcast();
}
