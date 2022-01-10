/*
 * POWER MONITOR SPLIT PHASE
 * Maxime MOREILLON
 * 
 * Board type: ESP32 DEv Board
 * 
 */

// Libraries
#include <WiFi.h>
#include <WiFiClient.h> 
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <WebServer.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>

#include "credentials.h";

#define HOSTNAME "currentMeasurement"

// Pin mapping
#define CT_1_PIN 36
#define CT_2_PIN 39

// MQTT
#define MQTT_BROKER_ADDRESS IPAddress(192, 168, 1, 2)
#define MQTT_PORT 1883
#define MQTT_STATUS_TOPIC "power/status"
#define MQTT_LAST_WILL "{\"state\":\"disconnected\"}"
#define MQTT_QOS 1
#define MQTT_RETAIN true

// WIFI
#define WIFI_CONNECTION_TIMEOUT 5000

// Web server
#define WWW_PORT 80
#define WS_PORT 8087

// ADC reading
#define SAMPLE_COUNT 2000.00
#define FILTER_CONSTANT 0.001
#define VCC 3.30
#define ADC_MAX 4095.00
#define BURDEN_VALUE 22.00
#define CT_RATIO 2000.00

// Timing
#define WS_SEND_PERIOD 1000
#define MQTT_PUBLISH_PERIOD 10000


class RmsCurrentMeasurement {
  private:
  
    int sample_count;
    float sum_of_squares;
    float lpf;
    

    void computeLpf(float in){
      this->lpf = (this->lpf)*(1.00 - FILTER_CONSTANT) + in * (FILTER_CONSTANT);
    }

    float computeHpf(float in){
      // A High pass filter is just a signal minus its low pass filtered version
      this->computeLpf(in);
      return in - (this->lpf);
    }
    
    float computeInstantaneousCurrent(float in){
      return in * (CT_RATIO * VCC) / (ADC_MAX * BURDEN_VALUE);
    }

    void computeRms(float in){
      // Increment sample counter
      this->sample_count ++;

      // Compute sum of squares
      this->sum_of_squares = sum_of_squares + (in * in);

      if(sample_count >= SAMPLE_COUNT-1) {

        // Compute mean then square
        this->RMS_current = sqrt(sum_of_squares / SAMPLE_COUNT);
        
        // reset everything
        
        this->sample_count = 0;
        this->sum_of_squares = 0;
      }
    }
    
    
  public:
    float RMS_current;
    
    RmsCurrentMeasurement() {
      
      // initialize variables
      this->RMS_current = 0;
      this->sample_count = 0;
      this->sum_of_squares = 0;
      this->lpf = 0;
    }
    
    void sample(int adc_pin) {
      // Raw measurement from ADC, [0-4096], centered around about 1800
      float ADC_reading = analogRead(adc_pin);

      // Removing low frequency component from signal to get high frequency component
      // i.e. remove constant offset
      float hpf = this->computeHpf(ADC_reading);

      // compute instantaneous current
      float InstantaneousCurrent = this->computeInstantaneousCurrent(hpf);

      // Compute RMS current
      this->computeRms(InstantaneousCurrent);

    }
};

WiFiClient wifi_client;
PubSubClient MQTT_client(wifi_client);
WebServer web_server(WWW_PORT);
WebSocketsServer ws_server = WebSocketsServer(WS_PORT);

RmsCurrentMeasurement phase_1_measurement;
RmsCurrentMeasurement phase_2_measurement;

int MQTT_submit_return = 0;

void setup() {
  // Mandatory initial delay
  delay(10);

  // Serial init
  Serial.begin(115200);
  Serial.println(); // Separate serial stream from initial gibberish
  Serial.println(F("Current consumption monitor"));
  
  wifi_setup();
  MQTT_setup();
  //ota_setup();
  web_server_setup();
  websocket_setup();
}

void loop() {
  wifi_connection_manager();
  MQTT_connection_manager();
  //ArduinoOTA.handle();
  web_server.handleClient();
  ws_server.loop();
  
  MQTT_client.loop();
  
  // conflict between measurements and mqtt
  phase_1_measurement.sample(CT_1_PIN);
  phase_2_measurement.sample(CT_2_PIN); 
  
  ws_send_periodically();
  periodic_MQTT_publish();
  
}
