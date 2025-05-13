#include <Arduino.h>
#include "LightStrip.h"


LightStrip lightStrip(NUM_PIXELS, PIN_NEO_PIXEL);
void setup() {
    Serial.begin(9600);
    Serial.println("Starting forward wave...");
    lightStrip.begin();
    lightStrip.clear();
    lightStrip.show();
}

void loop() {
    Serial.println("Forward wave");
        // Forward wave: single LED moves from 0 to NUM_PIXELS-1
    Serial.println("Wave forward");
    lightStrip.direction_wave(+1, 100, 50);
    delay(1000);

    // Backward wave: single LED moves from NUM_PIXELS-1 down to 0
    Serial.println("Wave backward");
    lightStrip.direction_wave(-1,100, 50);
    delay(1000);
}
