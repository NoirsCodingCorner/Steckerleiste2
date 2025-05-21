#include "attiny_communication.h"
#include <Wire.h>
#include <Arduino.h>  // Für micros() und Serial (nur falls Logging aktiviert)


AttinyCommunication::AttinyCommunication(int sdaPin, int sclPin) {
    if (sdaPin >= 0 && sclPin >= 0) {
        Wire.begin(sdaPin, sclPin);
    } else {
        Wire.begin();  // default SDA/SCL
    }
}

void AttinyCommunication::addAddress(uint8_t addr) {
    addresses.push_back(addr);
}

bool AttinyCommunication::getMovement(uint8_t addr) {
    unsigned long startMicros = micros();  // Zeitmessung starten

    Wire.requestFrom(addr, (uint8_t)1);    // 1 Byte vom Slave anfordern

    unsigned long durationMicros = micros() - startMicros;  // Dauer berechnen

#ifdef DEBUG_I2C_TIMING
    Serial.print("Addr 0x");
    Serial.print(addr, HEX);
    Serial.print(" antwortete in ");
    Serial.print(durationMicros);
    Serial.println(" µs.");
#endif

    if (Wire.available() < 1) {
        // Fehler oder keine Antwort
        return false;
    }
    return Wire.read() != 0;
}

void AttinyCommunication::pollAllMovements() {
    measurements.clear();
    measurements.reserve(addresses.size());
    for (auto addr : addresses) {
        measurements.push_back(getMovement(addr));
    }
}

const std::vector<bool>& AttinyCommunication::getMeasurements() const {
    return measurements;
}

const std::vector<uint8_t>& AttinyCommunication::getAddresses() const {
    return addresses;
}
