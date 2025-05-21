#define DEBUG_I2C_TIMING

#include <Arduino.h>
#include "attiny_communication.h"
#include "lightstrip.h"
#include "main_flow.h"  // Enthält die Klasse MainFlow

#define LED_BUILTIN 2

MainFlow flow;

unsigned long lastTriggeredTime = 0;
const unsigned long resetInterval = 10000;  // 10 Sekunden

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  // Sensor-Adressen setzen (0x05 und 0x06)
  flow.comm.addAddress(0x5C);

  flow.init();
  Serial.println("System gestartet.");
}

void loop() {
  flow.run();

  // Wenn Bewegung erkannt wurde, Zeitstempel setzen
  if (std::any_of(flow.current_values.begin(), flow.current_values.end(), [](bool b) { return b; })) {
    lastTriggeredTime = millis();
  }

  // Letzten Zustand nach definierter Zeit zurücksetzen
  if (millis() - lastTriggeredTime > resetInterval) {
    flow.last_values.clear();  // Vergessen des vorherigen Zustands
  }
  delay(100);  // 1 Sekunde Pause vor nächstem Lauf
}
