#include "main_flow.h"
#include "LightStrip.h" // Für NUM_PIXELS und die neuen fade-Methoden
#include <algorithm>    // für std::any_of

// Konstanten
static constexpr int FADE_STEP_DELAY = 25; // ms pro Helligkeitsschritt

void MainFlow::init()
{
    lightStrip.begin();
    lightStrip.warmth = 1.0;
    lightStrip.clear();
    lightStrip.show();

    lightSensor.setCutoff(20.0); // Beispiel: 20 Lux
    lightSensor.begin();
}

int MainFlow::direction() const
{
    int last_idx = -1;
    for (size_t i = 0; i < last_values.size(); ++i)
    {
        if (last_values[i])
        {
            last_idx = static_cast<int>(i);
            break;
        }
    }

    int curr_idx = -1;
    for (size_t i = 0; i < current_values.size(); ++i)
    {
        if (current_values[i])
        {
            curr_idx = static_cast<int>(i);
            break;
        }
    }

    if (last_idx < 0 || curr_idx < 0 || last_idx == curr_idx)
    {
        return 0;
    }
    return (curr_idx > last_idx) ? 1 : -1;
}

void MainFlow::measure()
{
    comm.pollAllMovements();
    last_values = current_values;
    current_values = comm.measurements;
}

void MainFlow::sendWave()
{
    int dir = direction();
    if (dir == 0)
        return;
    lightStrip.direction_wave(dir,
                              (NUM_PIXELS / static_cast<int>(current_values.size())) * static_cast<int>(current_values.size()),
                              50);
}

void MainFlow::run() {
    measure();

    bool motion = std::any_of(
        current_values.begin(),
        current_values.end(),
        [](bool v){ return v; }
    );
    bool dark = !lightSensor.isBright();

    Serial.print("Bewegung: ");
    Serial.print(motion);
    Serial.print(" | Dunkelheit: ");
    Serial.println(dark);

    int numSensors = static_cast<int>(current_values.size());
    if (numSensors <= 0) {
        // Wenn keine Sensoren, Strip komplett ausblenden
        lightStrip.setSegmentTarget(0, NUM_PIXELS - 1, 0);
        lightStrip.updateFades();
        return;
    }

    // 1) Länge jedes Segments = Strip-Länge / Sensoranzahl (Ganzzahl‐Division)
    int segLength = NUM_PIXELS / numSensors;
    if (segLength < 1) segLength = 1;

    if (motion && dark) {
        int raw     = analogRead(32);  // 0..4095
        int mapped  = map(raw, 0, 4095, 0, 128);
        Serial.print("Raw: ");
        Serial.print(raw);
        Serial.print(" | Mapped: ");
        Serial.println(mapped);
        uint8_t targetBrightness = static_cast<uint8_t>( constrain(brightness, 0, 128) );

        // 3) Für jeden Sensor‐Index i:
        for (int i = 0; i < numSensors; ++i) {
            int startIdx = i * segLength;
            int endIdx   = (i == numSensors - 1)
                           ? (NUM_PIXELS - 1)
                           : (startIdx + segLength - 1);

            if (current_values[i]) {
                // Sensor i meldet Bewegung → Segment auf targetBrightness
                Serial.print("Segment ");
                Serial.print(startIdx);
                Serial.print("–");
                Serial.print(endIdx);
                Serial.print(" animating to ");
                Serial.println(targetBrightness);

                lightStrip.setSegmentTarget(startIdx, endIdx, targetBrightness);
            } else {
                // Sensor i meldet kein Signal → Segment auf 0
                Serial.print("Segment ");
                Serial.print(startIdx);
                Serial.print("–");
                Serial.print(endIdx);
                Serial.println(" animating to 0");

                lightStrip.setSegmentTarget(startIdx, endIdx, 0);
            }
        }
    }
    else {
        // 4) Wenn keine Bewegung oder es ist hell → gesamtes Strip auf 0
        Serial.println("Kein Motion+dark: Fade gesamten Strip auf 0");
        lightStrip.setSegmentTarget(0, NUM_PIXELS - 1, 0);
    }

    // 5) Nicht‐blockierende Fade‐Schritte durchführen
    lightStrip.updateFades();
}