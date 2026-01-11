#include "WebServerController.h"
#include <Arduino.h>

WebServerController::WebServerController() : server(80), ledState(nullptr), LED_PIN(0), SERVO_PIN(5), lookingForwardAngle(0), lookingAtPlayersAngle(180), speaker(nullptr), gameFinished(nullptr), currentSpeed(0) {}

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

void WebServerController::setSpeakerPtr(Speaker* speakerPtr) {
  speaker = speakerPtr;
}

void WebServerController::setGameFinishedPtr(bool* gameFinishedPtr) {
  gameFinished = gameFinishedPtr;
}

void WebServerController::setupRoutes() {

  server.on("/look_forward", [this]() { this->onLookForward(); });
  server.on("/look_players", [this]() { this->onLookPlayers(); });
  server.on("/reset", [this]() { this->onReset(); });

  server.onNotFound([this]() {
    String uri = server.uri();
    if(gameFinished != nullptr && *gameFinished) {
      server.send(201, "text/plain", "Game finished. No further commands accepted.");
      return;
    }
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

void WebServerController::handleSpeed(int16_t value) {
  Motors::setSpeed(value);
  Motors::update();
  String response = "Speed value set to: ";
  response += value;
  server.send(200, "text/plain", response);
}

void WebServerController::handleTurn(uint16_t value) {
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
