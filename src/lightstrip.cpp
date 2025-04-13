#include <Adafruit_NeoPixel.h>
#include <vector>

#define PIN_NEO_PIXEL 16  // The ESP32 pin GPIO16 connected to NeoPixel
#define NUM_PIXELS 60     // Number of LEDs on the strip
#define WAVE_LENGTH 5     // Number of LEDs in the wave
#define DELAY_TIME 50     // Delay between steps

class LightStrip {
public:
  LightStrip(int numPixels, int pin)
    : strip(numPixels, pin, NEO_GRB + NEO_KHZ800), ledValues(numPixels, 0) {}

  void begin() {
    strip.begin();
    strip.clear();
    strip.show();
  }

  void clear() {
    for (int i = 0; i < ledValues.size(); i++) {
      ledValues[i] = 0;
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show();
  }

  void setLedValue(int index, uint8_t value) {
    if (index >= 0 && index < ledValues.size()) {
      ledValues[index] = value;
      strip.setPixelColor(index, strip.Color(value, 0, 0)); // Red brightness
    }
    show();
  }

  void show() {
    strip.show();
  }

  void updateWave(int waveHeadIndex) {
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

private:
  Adafruit_NeoPixel strip;
  std::vector<uint8_t> ledValues;
};

// Create global instance
LightStrip lightStrip(NUM_PIXELS, PIN_NEO_PIXEL);

/*
void setup() {
  lightStrip.begin();
}

void loop() {
  for (int i = 0; i < NUM_PIXELS + WAVE_LENGTH; i++) {
    lightStrip.updateWave(i);
    delay(DELAY_TIME);
  }
}
*/