#include "lightsensor.h"

Lightsensor::Lightsensor(uint8_t address)
    : sensor(address) {}

bool Lightsensor::begin() {
    Wire.begin();
    return sensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
}

bool Lightsensor::isReady() {
    return sensor.measurementReady();
}

float Lightsensor::measure() {
    /*
    int analogValue = analogRead(analogPin);
    if (analogValue > 10 && analogValue < 1020) {
        cutoff = analogValue / 10.0;
    }
    */
    return sensor.readLightLevel();
}

void Lightsensor::setCutoff(double newCutoff) {
    cutoff = newCutoff;
}

bool Lightsensor::isBright() {
    float lux = measure();
    return lux >= cutoff;
}

float Lightsensor::readAnalog() {
    if (analogPin >= 0) {
        return analogRead(analogPin);
    } else {
        return -1; // No valid pin
    }
}
