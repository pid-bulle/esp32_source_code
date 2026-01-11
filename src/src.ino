#include <WiFi.h>
#include "WebServerController.h"
#include "speaker.h"
#include "motors.h"

// WiFi credentials
const char* ssid = "Lenovo_hotspot";     // Replace with your WiFi SSID
const char* password = "qqqqqqqq"; // Replace with your WiFi password

// Web server controller
WebServerController webServer;

// LED blink on GPIO15 (non-blocking)
const int LED_PIN = 15; // GPIO 15
bool ledState = LOW;
unsigned long previousMillis = 0;
const unsigned long interval = 500; // toggle every 500 ms -> 1 Hz blink


const int SERVO_PIN = 5;
const int LOOKING_FORWARD_ANGLE = 180;
const int LOOKING_AT_PLAYERS_ANGLE = 0;

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
  webServer.setServoConfig(SERVO_PIN, LOOKING_FORWARD_ANGLE, LOOKING_AT_PLAYERS_ANGLE);
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
}
