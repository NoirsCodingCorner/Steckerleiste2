#pragma once

#include <Arduino.h>
#include <Wire.h>

/**
 * @brief Driver for a digital movement sensor over I2C.
 *
 * This class provides methods to initialize I2C communication and
 * detect movement events from a PIR or similar motion sensor.
 */
class MovementSensor {
public:
    /**
     * @brief Initialize the sensor communication.
     *
     * Configures I2C parameters and verifies that the sensor responds.
     * Must be called in setup().
     */
    void begin();

    /**
     * @brief Check for movement.
     *
     * Queries the sensor and returns a status code indicating whether
     * movement was detected, no movement occurred, or an error happened.
     *
     * @return +1 if movement was detected
     *         0 if no movement was detected within the timeout period
     *        -1 if an error occurred (e.g., communication timeout)
     */
    int8_t measureMovement();

private:
    /**
     * @brief Duration (in milliseconds) to hold the sensor output HIGH
     *        after a detection event, preventing further triggers
     *        until this lockout period expires.
     */
    static constexpr unsigned long DETECTION_HOLD_TIME_MS = 2000UL;

    /**
     * @brief Maximum duration (ms) to attempt a measurement before
     *        reporting a communication error.
     */
    static constexpr unsigned long MEASUREMENT_TIMEOUT_MS = 3000UL;

    /**
     * @brief GPIO pin connected to the movement sensor's digital output.
     */
    static constexpr int MOVEMENT_SENSOR_PIN = 1;

    unsigned long startTime = 0; //time since last high
};
