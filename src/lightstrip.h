#ifndef LIGHTSTRIP_H
#define LIGHTSTRIP_H

#include <Adafruit_NeoPixel.h>
#include <vector>
#include <Arduino.h>

// Macro definitions
#define PIN_NEO_PIXEL 16  // The ESP32 pin GPIO16 connected to NeoPixel
#define NUM_PIXELS 60     // Number of LEDs on the strip
#define WAVE_LENGTH 5     // Number of LEDs in the wave
#define DELAY_TIME 50     // Delay between steps

class LightStrip {
public:
    double warmth=0.5;
    // Constructor
    LightStrip(int numPixels, int pin);

    // Initialize the NeoPixel strip
    void begin();

    // Clear the strip (turn off all LEDs)
    void clear();

    // Set an individual LED's brightness (red channel)
    void setLedValue(int index, uint8_t value);

    // Update the physical strip with the current LED values
    void show();

    // Create a moving wave effect along the strip
    void updateWave(int waveHeadIndex);

private:
    Adafruit_NeoPixel strip;
    std::vector<uint8_t> ledValues;
};

// Global instance declaration
extern LightStrip lightStrip;

#endif // LIGHTSTRIP_H
