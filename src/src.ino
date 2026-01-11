#include <WiFi.h>
#include "WebServerController.h"
#include "speaker.h"
#include "motors.h"

// WiFi credentials
const char* ssid = "robot7";     // Replace with your WiFi SSID
const char* password = "okon123564"; // Replace with your WiFi password

// Web server controller
WebServerController webServer;

// LED blink on GPIO15 (non-blocking)
const int LED_PIN = 15; // GPIO 15
bool ledState = LOW;
unsigned long previousMillis = 0;
const unsigned long interval = 500; // toggle every 500 ms -> 1 Hz blink

Motors motors(
    18, 21, 19,   // pwmA, Ain1, Ain2
    23, 1, 22,    // pwmB, Bin1, Bin2
    2             // STBY
);


Speaker speaker(4);  // speaker on GPIO 4

void setup() {
  // Initialize speaker
  speaker.begin();
  speaker.soundGo();
  digitalWrite(4, 0);;
  speaker.detach();

  Serial.begin(115200);
  Serial.println("ESP32 ready — LED on GPIO15, speaker on GPIO4");

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

  motors.begin();
  motors.setWheelDistance(0.18);


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

      motors.drive(0.5, INFINITY);
    delay(2000);

    // Turn right, radius 0.3 m
    motors.drive(0.3, 0.3);
    delay(2000);

    // Turn left, radius -0.2 m
    motors.drive(0.3, -0.2);
    delay(2000);

    // Stop
    motors.stop();
    delay(1000);
}
