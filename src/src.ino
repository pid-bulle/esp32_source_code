#include <WiFi.h>
#include "WebServerController.h"
#include "speaker.h"

// WiFi credentials
const char* ssid = "HP_OMEN";     // Replace with your WiFi SSID
const char* password = "2a24%M95"; // Replace with your WiFi password

// Web server controller
WebServerController webServer;

// LED blink on GPIO15 (non-blocking)
const int LED_PIN = 15; // GPIO 15
bool ledState = LOW;
unsigned long previousMillis = 0;
const unsigned long interval = 500; // toggle every 500 ms -> 1 Hz blink

Speaker speaker(4);  // speaker on GPIO 4

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 ready — LED on GPIO15, speaker on GPIO26");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState);

  // Connect to WiFi
  Serial.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup HTTP server
  webServer.setLedStatePtr(&ledState, LED_PIN);
  webServer.setupRoutes();
  webServer.begin();


  // Initialize speaker
  speaker.begin();
  speaker.soundHalt();

}

void loop() {
  // Handle HTTP server requests
  webServer.handleClient();

  unsigned long currentMillis = millis();

  // non-blocking LED blink
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }
}
