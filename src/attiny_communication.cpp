// AttinyCommunication.cpp

#include "attiny_communication.h"
#include <Wire.h>

AttinyCommunication::AttinyCommunication(int sdaPin, int sclPin) {
    if (sdaPin >= 0 && sclPin >= 0) {
        Wire.begin(sdaPin, sclPin);
    } else {
        Wire.begin();           // default SDA/SCL
    }
    Wire.setClock(clockspeed);
}

void AttinyCommunication::addAddress(uint8_t addr) {
    addresses.push_back(addr);
}

bool AttinyCommunication::getMovement(uint8_t addr) {
    // Sensor returns a single byte: 0 for no movement, non-zero for movement
    Wire.requestFrom(addr, (uint8_t)1);
    if (Wire.available() < 1) {
        // error or no data
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
