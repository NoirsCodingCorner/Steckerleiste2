#ifndef MAINFLOW_H
#define MAINFLOW_H

#include <vector>
#include "attiny_communication.h"
#include "lightstrip.h"
#include "lightsensor.h"  // <--- NEU

class MainFlow {
public:
    std::vector<bool> current_values;
    std::vector<bool> last_values;
    int segs_per_sensor = 3;
    AttinyCommunication comm;
    LightStrip lightStrip;
    Lightsensor lightSensor;  // <--- NEU

    MainFlow()
        : comm(), lightStrip(NUM_PIXELS, PIN_NEO_PIXEL), lightSensor(0x23) {}

    void init();
    int direction() const;
    void measure();
    void sendWave();
    void run();
};

#endif // MAINFLOW_H
