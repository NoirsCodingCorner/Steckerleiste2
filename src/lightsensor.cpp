#include <BH1750.h>
#include <Wire.h>

class Lightsensor {
  

  public:
    BH1750 sensor;
    float cutoff = 0.0; // Default cutoff in lux (you can adjust)
    int analogPin = 4;


    Lightsensor(uint8_t address = 0x23) : sensor(address) {}

    bool begin() {
      Wire.begin();
      return sensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
    }

    bool isReady() {
      return sensor.measurementReady();
    }

    float measure() {
/*       int analogValue = analogRead(analogPin);

      // Check whether a potentiometer is connected (e.g. not permanently 0 or 1023)
      if (analogValue > 10 && analogValue < 1020) {
        cutoff = analogValue / 10.0;
      } */
    

      return sensor.readLightLevel();
    }


    void setCutoff(double newCutoff) {
      cutoff = newCutoff;
    }

    bool isBright() {
      float lux = measure();
      return lux >= cutoff;
    }

    float readAnalog() {
      if (analogPin >= 0) {
        return analogRead(analogPin);
      } else {
        return -1; // Error: No Pin is set
      }
    }
};