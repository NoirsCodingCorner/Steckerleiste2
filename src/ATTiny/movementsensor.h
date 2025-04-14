#include "Arduino.h"
#include <Wire.h>

// The potentiometer is connected to GPIO36 (Pin VP)
const int potPin = 0;

class MovementSensor {
  public:
    // I²C address for the sensor
    uint8_t address = 0x5C;
    double measuretimeout = 1000;
    void begin();
    double measure();
    bool active();
};