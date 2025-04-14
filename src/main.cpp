#include "Arduino.h"
#include <Wire.h>
#include "lightsensor.cpp"

Lightsensor lightsensor;

void setup() {
  Serial.begin(9600);

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
}
