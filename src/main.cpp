#include "Arduino.h"
#include <Wire.h>
#include "lightsensor.cpp"
#include "LightStrip.h"
Lightsensor lightsensor;

void setup() {
    Serial.begin(9600);
    lightStrip.begin();

  if (lightsensor.begin()) {
    Serial.println(F("BH1750 Initialized"));
  } else {
    Serial.println(F("Failed to initialize BH1750"));
  }
}

void loop() {
  if (lightsensor.isReady()) {
    float lux = lightsensor.read();
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");
  } 
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
