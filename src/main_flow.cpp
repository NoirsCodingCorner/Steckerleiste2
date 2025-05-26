#include "main_flow.h"
#include <algorithm>  // für std::any_of

void MainFlow::init() {
    lightStrip.begin();
    lightStrip.warmth = 1.0;
    lightStrip.clear();
    lightStrip.show();

    lightSensor.setCutoff(20.0);  // Beispielwert in Lux
    lightSensor.begin();
}

int MainFlow::direction() const {
    int last_idx = -1;
    for (size_t i = 0; i < last_values.size(); ++i) {
        if (last_values[i]) {
            last_idx = static_cast<int>(i);
            break;
        }
    }

    int curr_idx = -1;
    for (size_t i = 0; i < current_values.size(); ++i) {
        if (current_values[i]) {
            curr_idx = static_cast<int>(i);
            break;
        }
    }

    if (last_idx < 0 || curr_idx < 0 || last_idx == curr_idx) {
        return 0;
    }

    return (curr_idx > last_idx) ? 1 : -1;
}

void MainFlow::measure() {
    comm.pollAllMovements();
    last_values = current_values;
    current_values = comm.measurements;
}

void MainFlow::sendWave() {
    int dir = direction();
    if (dir == 0) return;
    lightStrip.direction_wave(dir, segs_per_sensor * static_cast<int>(current_values.size()), 50);
}

static constexpr int FADE_STEP_DELAY = 10;  // ms per brightness step

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

    if (motion && dark) {
        // --- 1) Read & map analog pot on pin 25 ---
        int raw = analogRead(25);                             // 0–4095 on ESP32
        int mapped = map(raw, 0, 4095, 0, 255);              
        uint8_t target = constrain(mapped, 0, 255);           // clamp just in case

        // --- 2) Fade up to that brightness ---
        lightStrip.fadeTo(target, FADE_STEP_DELAY);
    }
    else {
        // --- 3) Fade back down to zero ---
        lightStrip.fadeOut(FADE_STEP_DELAY);
    }
}

