#include "movementsensor.hpp"
#include <Wire.h>

void MovementSensor::begin() {
    // Initialize I²C and configure the sensor pin as input
    Wire.begin();
    pinMode(MOVEMENT_SENSOR_PIN, INPUT);
}

int8_t MovementSensor::measureMovement() {
    if (MEASUREMENT_TIMEOUT_MS > DETECTION_HOLD_TIME_MS){
        return -1; // the timeout is to fast! Timeout should not be faster than 
    }
    // Poll the sensor pin once 
        if (digitalRead(MOVEMENT_SENSOR_PIN) == HIGH) {
            if(DETECTION_HOLD_TIME_MS > (millis()-startTime)){
                return +2;  //same Movement detected
            }
            else {
                startTime = millis();
                return +1;  //new Movement detected
            }
        }
    // No movement detected
    return 0;
}
