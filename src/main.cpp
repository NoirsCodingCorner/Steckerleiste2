#include "LightStrip.h"

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
