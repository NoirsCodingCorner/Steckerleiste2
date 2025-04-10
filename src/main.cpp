#include <Adafruit_NeoPixel.h>

#define PIN_NEO_PIXEL 16  // The ESP32 pin GPIO16 connected to NeoPixel
#define NUM_PIXELS 60     // The number of LEDs (pixels) on NeoPixel LED strip
#define WAVE_LENGTH 5     // Number of LEDs in the "wave"
#define DELAY_TIME 50     // Delay between steps in milliseconds

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

void setup() {
  NeoPixel.begin();  // initialize NeoPixel strip object (REQUIRED)
  NeoPixel.clear();
  NeoPixel.show();
}

void loop() {
  for (int i = 0; i < NUM_PIXELS + WAVE_LENGTH; i++) {
    NeoPixel.clear(); // Clear all pixels

    for (int j = 0; j < WAVE_LENGTH; j++) {
      int pos = i - j;
      if (pos >= 0 && pos < NUM_PIXELS) {
        int brightness = 255 - (j * (255 / WAVE_LENGTH)); // Gradual fade
        NeoPixel.setPixelColor(pos, NeoPixel.Color(brightness, 0, 0)); // Red wave
      }
    }

    NeoPixel.show();
    delay(DELAY_TIME);
  }
}
