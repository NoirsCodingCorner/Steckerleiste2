#ifndef LIGHTSTRIP_H
#define LIGHTSTRIP_H

#include <Adafruit_NeoPixel.h>
#include <vector>
#include <Arduino.h>

// Standard‐Definitionen für Pin und Anzahl Pixel
#define PIN_NEO_PIXEL 16
#define NUM_PIXELS    58
#define WAVE_LENGTH 30     // Number of LEDs in the wave
#define DELAY_TIME 10     // Delay between steps

class LightStrip {
public:
    double warmth = 0.5;  // 0.0 = nur Grün, 1.0 = nur Rot, dazwischen Mischfarben

    // Konstruktor: Anzahl LEDs und Pin (Standard-Werte durch defines)
    LightStrip(int numPixels = NUM_PIXELS, int pin = PIN_NEO_PIXEL);

    // Muss vor Verwendung im setup() aufgerufen werden
    void begin();

    // Schaltet alle LEDs sofort aus
    void clear();

    // Setzt eine einzelne LED (Index) sofort auf 'value' (0–255), ohne Fading
    void setLedValue(int index, uint8_t value);

    // Sendet alle internen Farbwerte zum physikalischen Strip
    void show();

    // ---------------------------------------------
    // Blockierende Methoden (unverändert behalten)
    // ---------------------------------------------

    // Einfache Welle, die segmentweise über den Strip wandert (blockierend)
    void updateWave(int waveHeadIndex);

    // Setzt alle LEDs auf dieselbe Helligkeit (blockierend)
    void setAll(uint8_t brightness);

    // Direktrichtungs‐Welle über Block‐Delay (blockierend)
    void direction_wave(int waveDirection, double brightness = 0, int delayTime = 100);

    // Liest analogPin (0–4095), mapped auf 0–255 und setzt alle LEDs (blockierend)
    void setAllFromAnalog(int analogPin);

    // Fade aller LEDs auf targetBrightness (blockierend)
    void fadeTo(uint8_t targetBrightness, int stepDelay);

    // Fade out (alias für fadeTo(0,...))
    void fadeOut(int stepDelay);

    // ---------------------------------------------
    // Neu: Nicht‐blockierende Segment‐Fading‐API
    // ---------------------------------------------

    // 1) Für jedes Segment willst du speichern, auf welchen Wert es „hinfaden“ soll.
    //    Das machen wir, indem wir targetValues[...] füllen.
    //    Dieser Aufruf schreibt für [startIndex..endIndex] denselben Ziel‐Helligkeitswert rein.
    void setSegmentTarget(int startIndex, int endIndex, uint8_t targetBrightness);

    // 2) In loop() muss dann regelmäßig updateFades() aufgerufen werden.
    //    Es durchläuft alle LEDs und passt jede um ±1 pro Schleifendurchlauf (kein delay).
    void updateFades();

private:
    Adafruit_NeoPixel strip;
    std::vector<uint8_t> ledValues;    // aktueller Ist‐Helligkeitswert (0–255) pro LED
    std::vector<uint8_t> targetValues; // Soll‐Helligkeitswert (0–255) pro LED
};

#endif // LIGHTSTRIP_H
