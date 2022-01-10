#include <Arduino.h>

#define SAMPLE_COUNT 2000.00 // Number of samples to compute the RMS
#define FILTER_CONSTANT 0.001
#define VCC 3.30 // ESP32 VCC
#define ADC_MAX 4095.00 // Maximum value of the ADC
#define BURDEN_VALUE 22.00 // Resistance of the Burdern resistor
#define CT_RATIO 2000.00 // Coil ratio of the CT sensor

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
