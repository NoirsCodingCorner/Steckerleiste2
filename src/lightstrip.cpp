#include "LightStrip.h"

// Constructor implementation, using initializer list to setup the NeoPixel object and ledValues vector
LightStrip::LightStrip(int numPixels, int pin)
    : strip(numPixels, pin, NEO_GRB + NEO_KHZ800), ledValues(numPixels, 0)
{
}

// Initialize the NeoPixel strip
void LightStrip::begin() {
    strip.begin();
    strip.clear();
    strip.show();
}

// Clear all LED values and update the strip to turn off all LEDs
void LightStrip::clear() {
    strip.clear();
}

// Set the brightness of a specific LED (red channel) and update the strip
void LightStrip::setLedValue(int index, uint8_t value) {
    if (index >= 0 && index < ledValues.size()) {
        ledValues[index] = value;
        strip.setPixelColor(index, strip.Color((double)(value)*warmth, (double)(value)*(1-warmth), 0));  // Only adjust the red brightness
        //Serial.println("LED set to " + String(value)+ " at index " + String(index)+ "in rbg: red:" + String((double)(value)*warmth) + " green:" + String((double)(value)*(1-warmth)));
    }
    show();
}

// Update the physical LED strip display
void LightStrip::show() {
    strip.show();
}

// Create a moving wave effect by updating a range of LEDs' brightness
void LightStrip::updateWave(int waveHeadIndex) {
    clear();
    for (int j = 0; j < WAVE_LENGTH; j++) {
        int pos = waveHeadIndex - j;
        if (pos >= 0 && pos < ledValues.size()) {
            int brightness = 255 - (j * (255 / WAVE_LENGTH));
            setLedValue(pos, brightness);
        }
    }
    show();
}

// Global instance definition
LightStrip lightStrip(NUM_PIXELS, PIN_NEO_PIXEL);

/*
void setup() {
    Serial.begin(115200);
    lightStrip.begin();
}

void loop() {
 for (int j = 0; j < 57; j++)
 {
    lightStrip.clear();
    for (int i = 0; i < 100; i++)
    {
        lightStrip.warmth=i/100.0;
        lightStrip.setLedValue(j,255/4);
        lightStrip.setLedValue(j+1,255/2);
        lightStrip.setLedValue(j+2,255/4);
        delay(DELAY_TIME);
    }
    for (int i = 100; i >= 0; i--)
    {
        lightStrip.warmth=i/100.0;
        lightStrip.setLedValue(j,255/4);
        lightStrip.setLedValue(j+1,255/2);
        lightStrip.setLedValue(j+2,255/4);
        delay(DELAY_TIME);
    }
 } 
}
*/