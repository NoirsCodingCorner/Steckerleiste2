#include "MovementSensor.h"
MovementSensor sensor = MovementSensor();

// This function will be called when a master requests data.
void onRequest() {
    // Get the current measurement from the sensor.
    // Note: On many Arduino platforms, double is the same as float (4 bytes).
    double measurement = sensor.measure();
    // Send the measurement as binary data.
    // Make sure that the master interprets the byte order and size as expected.
    Wire.write((byte*) &measurement, sizeof(measurement));
}

void setup() {
    Serial.begin(115200);
    sensor.begin();
    
    // Initialize I²C as a slave device with the sensor's address.
    Wire.begin(sensor.address);
    // Register the callback function that sends sensor data when requested.
    Wire.onRequest(onRequest);
}

void loop() {
    // Optionally print measurement to the Serial Monitor for debugging.
    double measurement = sensor.measure();
    Serial.println("Measurement: " + String(measurement));
    delay(500);
}