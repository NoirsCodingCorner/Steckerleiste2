#include "MovementSensor.hpp"
#include <Wire.h>

MovementSensor sensor;

// Diese Funktion wird aufgerufen, wenn der Master Daten vom Slave anfordert
void onRequest() {
    // Bewegung messen
    double measurement = sensor.measureMovement();
    
    // Messwert als Binärdaten senden
    Wire.write((byte*) &measurement, sizeof(measurement));
}

void setup() {
    Serial.begin(115200);

    // Standard-I2C-Adresse des Bewegungssensors
    constexpr uint8_t I2C_ADDRESS = 0x5C;

    sensor.begin();

    // I2C als Slave initialisieren
    Wire.begin(I2C_ADDRESS);

    // Callback registrieren für Datenanforderungen vom Master
    Wire.onRequest(onRequest);
}

void loop() {
    // Optional: Messwert im Seriellen Monitor ausgeben
    double measurement = sensor.measure();
    // Serial.println("Measurement: " + String(measurement));
    delay(500);
}
