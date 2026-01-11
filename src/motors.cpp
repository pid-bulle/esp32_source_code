#include "motors.h"

// ---------- Static member definitions ----------
uint8_t Motors::_pwmA;
uint8_t Motors::_ain1;
uint8_t Motors::_ain2;
uint8_t Motors::_pwmB;
uint8_t Motors::_bin1;
uint8_t Motors::_bin2;
uint8_t Motors::_stby;

int16_t Motors::_basePwm = 0;
float   Motors::_turnRadius = INFINITY;
float   Motors::_wheelDist = 0.12;  // default 12 cm

// ---------- Initialization ----------
void Motors::begin(
  uint8_t pwmA, uint8_t ain1, uint8_t ain2,
  uint8_t pwmB, uint8_t bin1, uint8_t bin2,
  uint8_t stby
) {
  _pwmA = pwmA; _ain1 = ain1; _ain2 = ain2;
  _pwmB = pwmB; _bin1 = bin1; _bin2 = bin2;
  _stby = stby;

  pinMode(_ain1, OUTPUT);
  pinMode(_ain2, OUTPUT);
  pinMode(_bin1, OUTPUT);
  pinMode(_bin2, OUTPUT);
  pinMode(_stby, OUTPUT);

  digitalWrite(_stby, HIGH);  // enable driver
  stop();
}

// ---------- Public API ----------
void Motors::setSpeed(int16_t pwm) {
  _basePwm = constrain(pwm, -255, 255);
}

void Motors::setTurnRadius(float radius) {
  _turnRadius = radius;
}

void Motors::setWheelDistance(float meters) {
  _wheelDist = meters;
}

void Motors::standby(bool enable) {
  digitalWrite(_stby, enable ? LOW : HIGH);
}

void Motors::stop() {
  driveMotor(_pwmA, _ain1, _ain2, 0);
  driveMotor(_pwmB, _bin1, _bin2, 0);
}

// ---------- Core update logic ----------
void Motors::update() {
  float leftFactor = 1.0;
  float rightFactor = 1.0;

  if (_turnRadius == 0.0f) {
    // In-place rotation
    leftFactor  = -1.0;
    rightFactor =  1.0;
  }
  else if (!isinf(_turnRadius)) {
    // Differential drive curvature
    leftFactor  = (_turnRadius - _wheelDist / 2.0f) / _turnRadius;
    rightFactor = (_turnRadius + _wheelDist / 2.0f) / _turnRadius;
  }

  int16_t pwmL = constrain(_basePwm * leftFactor,  -255, 255);
  int16_t pwmR = constrain(_basePwm * rightFactor, -255, 255);

  driveMotor(_pwmA, _ain1, _ain2, pwmL);
  driveMotor(_pwmB, _bin1, _bin2, pwmR);
}

// ---------- Low-level motor drive ----------
void Motors::driveMotor(
  uint8_t pwmPin, uint8_t in1, uint8_t in2, int16_t pwm
) {
  if (pwm > 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else if (pwm < 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    pwm = -pwm;
  } else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }

  pwm = constrain(pwm, 0, 255);

  // Latest ESP32 LEDC API (pin-based)
  ledcAttach(pwmPin, 1000, _pwmResolution);
  ledcWrite(pwmPin, pwm);
}
