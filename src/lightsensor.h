#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#include <Wire.h>
#include <BH1750.h>
#include <Arduino.h>

class Lightsensor {
public:
    BH1750 sensor;
    float cutoff = 0.0;       // Lux-Schwelle
    int analogPin = 25;        // Optionaler analoger Poti-Eingang

    // Konstruktor mit optionaler Adresse (0x23 oder 0x5C)
    Lightsensor(uint8_t address = 0x23);

    // Initialisiert den Sensor
    bool begin();

    // Prüft, ob Messwert bereit ist
    bool isReady();

    // Führt eine Lux-Messung durch
    float measure();

    // Setzt die Schwelle für isBright()
    void setCutoff(double newCutoff);

    // Gibt true zurück, wenn Helligkeit >= cutoff
    bool isBright();

    // Liest analogen Wert (z. B. von Poti)
    float readAnalog();
};

#endif // LIGHTSENSOR_H
