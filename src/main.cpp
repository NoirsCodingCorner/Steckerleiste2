#include "LightStrip.h"

void setup() {
    Serial.begin(115200);
    lightStrip.begin();
}

void loop() {

    for (int i = 0; i < 100; i++)
    {
        lightStrip.warmth=i/100.0;
        lightStrip.setLedValue(0,255/4);
        lightStrip.setLedValue(1,255/2);
        lightStrip.setLedValue(2,255/4);
        delay(100);
    }
    for (int i = 100; i >= 0; i--)
    {
        lightStrip.warmth=i/100.0;
        lightStrip.setLedValue(0,255/4);
        lightStrip.setLedValue(1,255/2);
        lightStrip.setLedValue(2,255/4);
        delay(100);
    }
    
}
