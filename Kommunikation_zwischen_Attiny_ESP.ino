#include <Wire.h>

class MovementSensor {
public:
    void begin();
    bool measureMovement(); // Änderung: Rückgabewert ist jetzt bool

private:
    static constexpr unsigned long DETECTION_HOLD_TIME_MS = 10UL;
    static constexpr unsigned long MEASUREMENT_TIMEOUT_MS = 300UL;

    static constexpr uint8_t MOVEMENT_SENSOR_PIN = 1;

    unsigned long startTime = 0;
    bool motionDetected = false;
};

void MovementSensor::begin() {
    pinMode(MOVEMENT_SENSOR_PIN, INPUT);
}

bool MovementSensor::measureMovement() {
    if (digitalRead(MOVEMENT_SENSOR_PIN) == HIGH) {
        if (!motionDetected || (millis() - startTime > DETECTION_HOLD_TIME_MS)) {
            startTime = millis();
            motionDetected = true;
        }
    } else if (millis() - startTime > DETECTION_HOLD_TIME_MS) {
        motionDetected = false;
    }

    return motionDetected;
}

MovementSensor sensor;

void onRequest() {
    bool measurement = sensor.measureMovement();
    Wire.write(static_cast<uint8_t>(measurement)); // 1 Byte senden (0 oder 1)
}

void setup() {
    static constexpr uint8_t I2C_ADDRESS = 0x5C;

    sensor.begin();

    Wire.begin(I2C_ADDRESS);
    Wire.onRequest(onRequest);
}

void loop() {
    delay(50); // Optional: Debugging oder andere Aufgaben
}
