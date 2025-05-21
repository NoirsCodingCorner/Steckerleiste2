#ifndef MAINFLOW_H
#define MAINFLOW_H

#include <vector>
#include "attiny_communication.h"
#include "lightstrip.h"

class MainFlow {
public:
    std::vector<bool> current_values;
    std::vector<bool> last_values;
    int segs_per_sensor = 3;
    AttinyCommunication comm;
    LightStrip lightStrip;

    MainFlow()
        : comm(), lightStrip(NUM_PIXELS, PIN_NEO_PIXEL) {}

    void init();

    /**
     * @brief Determine movement direction based on current_values vs last_values.
     * 
     * Scans for the first active sensor in each vector:
     * - If either vector has no `true` entries, returns 0.
     * - If the active index has increased since last_values, returns +1.
     * - If it has decreased, returns -1.
     * - If it's the same, returns 0.
     */
    int direction() const;

    void measure();

    void sendWave();

    void run();
};

#endif // MAINFLOW_H
