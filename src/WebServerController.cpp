#include "WebServerController.h"
#include <Arduino.h>

WebServerController::WebServerController() : server(80), ledState(nullptr), LED_PIN(0), SERVO_PIN(5), lookingForwardAngle(0), lookingAtPlayersAngle(180), motors(nullptr), speaker(nullptr), currentSpeed(0) {}

void WebServerController::begin() {
  servo.attach(SERVO_PIN);
  servo.write(lookingForwardAngle);
  Serial.print("Servo attached to GPIO ");
  Serial.println(SERVO_PIN);

  server.begin();
  Serial.println("HTTP server started");
}

void WebServerController::handleClient() {
  server.handleClient();
}

void WebServerController::setLedStatePtr(bool* ledStatePtr, int ledPin) {
  ledState = ledStatePtr;
  LED_PIN = ledPin;
}

void WebServerController::setServoConfig(int servoPin, int forwardAngle, int playersAngle) {
  SERVO_PIN = servoPin;
  lookingForwardAngle = forwardAngle;
  lookingAtPlayersAngle = playersAngle;
}

void WebServerController::setMotorsPtr(Motors* motorsPtr) {
  motors = motorsPtr;
}

void WebServerController::setSpeakerPtr(Speaker* speakerPtr) {
  speaker = speakerPtr;
}

void WebServerController::setupRoutes() {

  server.on("/look_forward", [this]() { this->onLookForward(); });
  server.on("/look_players", [this]() { this->onLookPlayers(); });
  server.on("/reset", [this]() { this->onReset(); });

  server.onNotFound([this]() {
    String uri = server.uri();

    if (uri.startsWith("/led/")) {
      int value = uri.substring(5).toInt();
      this->handleLed(value);
    } else if (uri.startsWith("/speed/")) {
      int value = uri.substring(7).toInt();
      this->handleSpeed(value);
    } else if (uri.startsWith("/turn/")) {
      int value = uri.substring(6).toInt();
      this->handleTurn(value);
    } else if (uri.startsWith("/sound/")) {
      int value = uri.substring(7).toInt();
      this->handleSound(value);
    } else {
      server.send(404, "text/plain", "Not Found");
    }
  });
}

void WebServerController::onLookForward() {
  servo.write(lookingForwardAngle);
  server.send(200, "text/plain", "Look forward command received");
}

void WebServerController::onLookPlayers() {
  servo.write(lookingAtPlayersAngle);
  server.send(200, "text/plain", "Look players command received");
}

void WebServerController::onReset() {
  server.send(200, "text/plain", "Reset command received");
}

void WebServerController::handleLed(uint16_t value) {
  String response = "LED value set to: ";
  response += value;
  server.send(200, "text/plain", response);
}

void WebServerController::handleSpeed(uint16_t value) {
  currentSpeed = value;  // zapamiętaj bieżącą prędkość
  if (motors != nullptr) {
    // value 0-255, jedź prosto (INFINITY = brak skrętu)
    float speed = value / 255.0;  // normalizuj do 0.0-1.0
    motors->drive(speed, INFINITY);
  }
  String response = "Speed value set to: ";
  response += value;
  server.send(200, "text/plain", response);
}

void WebServerController::handleTurn(uint16_t value) {
  if (motors != nullptr) {
    // value 0 = ostry skręt w lewo, 128 = prosto, 255 = ostry skręt w prawo
    // Używamy currentSpeed jako prędkości bazowej
    // value < 128: lewy wolniej, prawy szybciej (skręt w lewo)
    // value > 128: lewy szybciej, prawy wolniej (skręt w prawo)

    int16_t diff = (int16_t)value - 128;  // -128 do +127

    // Skaluj diff proporcjonalnie do currentSpeed
    int16_t leftSpeed = currentSpeed + diff;
    int16_t rightSpeed = currentSpeed - diff;

    // Ogranicz do zakresu -255 do 255
    leftSpeed = constrain(leftSpeed, -255, 255);
    rightSpeed = constrain(rightSpeed, -255, 255);

    motors->setMotorA(leftSpeed);
    motors->setMotorB(rightSpeed);
  }
  String response = "Turn value set to: ";
  response += value;
  server.send(200, "text/plain", response);
}

void WebServerController::handleSound(uint16_t value) {
  if (speaker != nullptr) {
    speaker->begin();
    switch (value) {
      case 1:
        speaker->soundWin();
        break;
      case 2:
        speaker->soundLoss();
        break;
      case 3:
        speaker->soundGo();
        break;
      case 4:
        speaker->soundHalt();
        break;
    }
    speaker->detach();
  }
  String response = "Sound value set to: ";
  response += value;
  server.send(200, "text/plain", response);
}
