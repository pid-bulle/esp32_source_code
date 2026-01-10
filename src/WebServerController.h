#ifndef WEB_SERVER_CONTROLLER_H
#define WEB_SERVER_CONTROLLER_H

#include <WebServer.h>

class WebServerController {
public:
  WebServerController();
  void begin();
  void handleClient();
  void setupRoutes();

  // Reference to LED state for control
  void setLedStatePtr(bool* ledStatePtr, int ledPin);

private:
  WebServer server;
  bool* ledState;
  int LED_PIN;

  void onLookForward();
  void onLookPlayers();
  void onReset();
  void handleLed(uint16_t value);
  void handleSpeed(uint16_t value);
  void handleTurn(uint16_t value);
  void handleSound(uint16_t value);
};

#endif
