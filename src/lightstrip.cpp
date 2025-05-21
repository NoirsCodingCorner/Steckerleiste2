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

void LightStrip::setAll(uint8_t brightness) {
    for (int i = 0; i < strip.numPixels(); ++i) {
        ledValues[i] = brightness;
        strip.setPixelColor(i, strip.Color((double)(brightness) * warmth, (double)(brightness) * (1 - warmth), 0));
    }
    show();
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

void LightStrip::setAllFromAnalog(int analogPin) {
    int analogValue = analogRead(analogPin);
    Serial.println("Analog Value: " + String(analogValue));

    // Annahme: ESP32 mit 12-bit ADC → Wertebereich 0–4095 bei 0–3.3V oder 0–4.0V
    // Wenn du Arduino nutzt, verwende 1023 statt 4095
    int brightness = map(analogValue, 0, 4095, 0, 255);
    brightness = constrain(brightness, 0, 255);  // Sicherheitshalber begrenzen

    setAll(static_cast<uint8_t>(brightness));
}

// In LightStrip.cpp:

void LightStrip::direction_wave(int wavedirection, double birghtness=0, int delaytime=100) {
    size_t n = ledValues.size();

    if (wavedirection > 0) {
        // forward: single LED moves from 0 to n-1
        for (size_t i = 0; i < n; ++i) {
            clear();                    // turn off all LEDs
            setLedValue(i, birghtness);        // turn on just this one
            delay(delaytime);                // hold for 1 second
        }
    }
    else if (wavedirection < 0) {
        // backward: single LED moves from n-1 down to 0
        for (int i = int(n) - 1; i >= 0; --i) {
            clear();
            setLedValue(i, birghtness);
            delay(delaytime);
        }
    }
    else {
        // no movement: just clear
        clear();
        show();
    }
}

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