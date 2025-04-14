#include "movementsensor.h"

void MovementSensor::begin() {
    // In this case we use the internal pull-up for the pot pin
    pinMode(potPin, INPUT_PULLUP);
}

double MovementSensor::measure() {
    // Returns an analog value from the specified potentiometer pin
    return analogRead(potPin);
}

// Optional: add a basic active check if needed
bool MovementSensor::active() {
    // For example, return true if the sensor is initialized correctly.
    return true;
}