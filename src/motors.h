#pragma once
#include <Arduino.h>

class Motors {
public:
    Motors(
        uint8_t pwmA, uint8_t ain1, uint8_t ain2,
        uint8_t pwmB, uint8_t bin1, uint8_t bin2,
        uint8_t stby
    );

    void begin();

    // Low-level motor controls
    void setMotorA(int16_t speed);  // speed: -255..255
    void setMotorB(int16_t speed);  // speed: -255..255
    void stop();                     // stop both motors

    // High-level differential drive control
    // linearVel = forward speed (m/s)
    // turnRadius = turning radius (m) +ve = right turn, -ve = left turn
    void drive(float linearVel, float turnRadius);

    // Robot configuration
    void setWheelDistance(float d); // distance between left & right wheels (m)

private:
    uint8_t _pwmA, _ain1, _ain2;
    uint8_t _pwmB, _bin1, _bin2;
    uint8_t _stby;
    uint8_t _pwmResolution = 8;

    float _wheelDistance = 0.12; // default distance between wheels in meters

    void applyMotor(int16_t pwm, uint8_t pwmPin, uint8_t pin1, uint8_t pin2);
};
