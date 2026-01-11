#include "motors.h"

Motors::Motors(
    uint8_t pwmA, uint8_t ain1, uint8_t ain2,
    uint8_t pwmB, uint8_t bin1, uint8_t bin2,
    uint8_t stby
) : _pwmA(pwmA), _ain1(ain1), _ain2(ain2),
    _pwmB(pwmB), _bin1(bin1), _bin2(bin2),
    _stby(stby) { }

void Motors::begin() {
    pinMode(_ain1, OUTPUT);
    pinMode(_ain2, OUTPUT);
    pinMode(_bin1, OUTPUT);
    pinMode(_bin2, OUTPUT);
    pinMode(_stby, OUTPUT);

    digitalWrite(_stby, HIGH);  // exit standby

    // Setup PWM on PWM pins using pinMode for PWM
    pinMode(_pwmA, OUTPUT);
    pinMode(_pwmB, OUTPUT);

    // Setup LEDC for PWM
    bool attachA = ledcAttach(_pwmA, 1000, _pwmResolution);
    bool attachB = ledcAttach(_pwmB, 1000, _pwmResolution);

    Serial.print("ledcAttach pwmA (GPIO ");
    Serial.print(_pwmA);
    Serial.print("): ");
    Serial.println(attachA ? "OK" : "FAIL");

    Serial.print("ledcAttach pwmB (GPIO ");
    Serial.print(_pwmB);
    Serial.print("): ");
    Serial.println(attachB ? "OK" : "FAIL");

    // Test: set 50% duty cycle directly
    ledcWrite(_pwmA, 128);
    ledcWrite(_pwmB, 128);
    Serial.println("PWM set to 50% on both channels");

    // ensure stopped
    // setMotorA(0);
    // setMotorB(0);
}

void Motors::applyMotor(int16_t pwm, uint8_t pwmPin, uint8_t pin1, uint8_t pin2) {
    if (pwm > 0) {
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
    } else if (pwm < 0) {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
        pwm = -pwm;
    } else {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW);
    }

    // limit to 0–255
    if (pwm > 255) pwm = 255;

    // write PWM to the actual PWM pin
    ledcWrite(pwmPin, pwm);
}

void Motors::setMotorA(int16_t speed) {
    applyMotor(speed, _pwmA, _ain1, _ain2);
}

void Motors::setMotorB(int16_t speed) {
    applyMotor(speed, _pwmB, _bin1, _bin2);
}

void Motors::stop() {
    setMotorA(0);
    setMotorB(0);
}

// Robot kinematics: convert linear velocity and turning radius to wheel speeds
void Motors::drive(float linearVel, float turnRadius) {
    float vL, vR;

    if (turnRadius == 0) {
        // in-place rotation: left and right opposite
        vL = -linearVel;
        vR = linearVel;
    } else if (isinf(turnRadius)) {
        // straight line
        vL = linearVel;
        vR = linearVel;
    } else {
        // differential drive: vL = v*(R - d/2)/R, vR = v*(R + d/2)/R
        vL = linearVel * (turnRadius - _wheelDistance / 2.0) / turnRadius;
        vR = linearVel * (turnRadius + _wheelDistance / 2.0) / turnRadius;
    }

    // Convert linear velocity (m/s) to PWM (-255..255)
    // scale factor: max speed corresponds to full PWM
    float maxSpeed = 1.0; // m/s, tune according to your motors
    int16_t pwmL = (int16_t)(vL / maxSpeed * 255);
    int16_t pwmR = (int16_t)(vR / maxSpeed * 255);

    setMotorA(pwmL);
    setMotorB(pwmR);
}

void Motors::setWheelDistance(float d) {
    _wheelDistance = d;
}
