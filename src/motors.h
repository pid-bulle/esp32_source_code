#pragma once
#include <Arduino.h>

class Motors {
public:
  // TB6612FNG wiring
  static void begin(
    uint8_t pwmA, uint8_t ain1, uint8_t ain2,
    uint8_t pwmB, uint8_t bin1, uint8_t bin2,
    uint8_t stby
  );

  // State setters
  static void setSpeed(int16_t pwm);        // -255..255 (forward/backward)
  static void setTurnRadius(float radius);  // meters, +right, -left, INF = straight

  // Apply current state to motors
  static void update();

  // Safety
  static void stop();
  static void standby(bool enable);

  // Robot geometry
  static void setWheelDistance(float meters);

private:
  // Pins
  static uint8_t _pwmA, _ain1, _ain2;
  static uint8_t _pwmB, _bin1, _bin2;
  static uint8_t _stby;

  // State
  static int16_t _basePwm;     // -255..255
  static float   _turnRadius;  // meters
  static float   _wheelDist;   // meters

  static constexpr uint8_t _pwmResolution = 8;

  static void driveMotor(uint8_t pwmPin, uint8_t in1, uint8_t in2, int16_t pwm);
};
