/*
 * POWER MONITOR
 * Maxime MOREILLON
 * 
 * Board type: ESP32 DEv Board
 * 
 */

#include "IotKernel.h"
#include "RmsCurrentMeasurement.cpp"


// Pin mapping
#define CT_1_PIN 36
#define CT_2_PIN 39

// MQTT
#define MQTT_POWER_STATUS_TOPIC "power/status"

// Timing
#define MQTT_PUBLISH_PERIOD 10000


IotKernel iot_kernel("power","0.1.0"); 

RmsCurrentMeasurement phase_1_measurement;
RmsCurrentMeasurement phase_2_measurement;

void setup() {
  iot_kernel.init();
  mqtt_config();
}

void loop() {

  iot_kernel.loop();

  phase_1_measurement.sample(CT_1_PIN);
  phase_2_measurement.sample(CT_2_PIN); 
  
  periodic_MQTT_publish();
}
