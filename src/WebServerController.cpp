#include "WebServerController.h"
#include <Arduino.h>

WebServerController::WebServerController() : server(80), ledState(nullptr), LED_PIN(0) {}

void WebServerController::begin() {
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
  server.send(200, "text/plain", "Look forward command received");
}

void WebServerController::onLookPlayers() {
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
  String response = "Sound value set to: ";
  response += value;
  server.send(200, "text/plain", response);
}
