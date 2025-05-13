
/// The mainflow consists of multiple things that need to happen
/// 1. Get the current motion of one of the sensors with its index
/// 2. Get the current light level to register if has to be on
/// 3. Switch the segement on that corresponds to the index (with swift animation)
/// 4. Remember the last activated movement sensor -> animation direction
/// 5. After a while reset the last remembered value

#include <vector>
#include "attiny_communication.h"
#include "lightstrip.h"

class MainFlow {
    public:
    std::vector<bool> current_values;
    std::vector<bool> last_values;
    int segs_per_sensor = 3;
    AttinyCommunication comm=AttinyCommunication();
    LightStrip lightStrip = LightStrip(NUM_PIXELS, PIN_NEO_PIXEL); 

    void init() {
        lightStrip.begin();

        lightStrip.warmth=1.0;
        lightStrip.clear();
        lightStrip.show();
    }

    /**
     * @brief Determine movement direction based on current_values vs last_values.
     * 
     * Scans for the first active sensor in each vector:
     * - If either vector has no `true` entries, returns 0.
     * - If the active index has increased since last_values, returns +1.
     * - If it has decreased, returns -1.
     * - If it's the same, returns 0.
     */
    int direction() const {
        // find first active index in last_values
        int last_idx = -1;
        for (size_t i = 0; i < last_values.size(); ++i) {
            if (last_values[i]) {
                last_idx = static_cast<int>(i);
                break;
            }
        }

        // find first active index in current_values
        int curr_idx = -1;
        for (size_t i = 0; i < current_values.size(); ++i) {
            if (current_values[i]) {
                curr_idx = static_cast<int>(i);
                break;
            }
        }

        // if either has no active sensor, or no change, return 0
        if (last_idx < 0 || curr_idx < 0 || last_idx == curr_idx) {
            return 0;
        }

        // forward if index increased, backward if decreased
        return (curr_idx > last_idx) ? 1 : -1;
    }
    

    void measure() {
        comm.pollAllMovements();
        last_values = current_values;
        current_values = comm.measurements;
    }

    void sendWave() {
        int direction = this->direction();
        if (direction == 0) {
            return;
        }
        lightStrip.direction_wave(direction,100,50);
    }
    
    void run() {
        measure();
        sendWave();
    }
};