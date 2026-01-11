#include <WiFi.h>
#include "WebServerController.h"
#include "speaker.h"
#include "motors.h"
#include "led_ring.h"

// WiFi credentials
// const char* ssid = "Lenovo_hotspot";     // Replace with your WiFi SSID
// const char* password = "qqqqqqqq"; // Replace with your WiFi password
const char* ssid = "robot7";     // Replace with your WiFi SSID
const char* password = "okon123564"; // Replace with your WiFi password

// Web server controller
WebServerController webServer;

// LED blink on GPIO15 (non-blocking)
const int LED_PIN = 26; // GPIO 15
bool ledState = LOW;
unsigned long previousMillis = 0;
const unsigned long interval = 500; // toggle every 500 ms -> 1 Hz blink


const int SERVO_PIN = 5;
const int LOOKING_FORWARD_ANGLE = 180;
const int LOOKING_AT_PLAYERS_ANGLE = 0;

const int BUTTON_PIN = 25; // Button on D25
const int CONTROL_LED_PIN = 33; // LED on D33

Speaker speaker(4);  // speaker on GPIO 4

LedRing ledRing(35);  // WS2812B LED ring on GPIO 35

static bool gameFinished = false;
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

  // Setup button and control LED
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Button with internal pull-up
  pinMode(CONTROL_LED_PIN, OUTPUT);
  digitalWrite(CONTROL_LED_PIN, LOW);

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
  webServer.setSpeakerPtr(&speaker);
  webServer.setGameFinishedPtr(&gameFinished);
  webServer.setupRoutes();
  webServer.begin();

  Motors::begin(
      18, 21, 19,   // pwmA, Ain1, Ain2
      23, 1, 22,    // pwmB, Bin1, Bin2
      2             // STBY
  );
  Motors::setWheelDistance(0.18);
  Motors::setSpeed(0);
  Motors::setTurnRadius(INFINITY);
  Motors::update();

  // Initialize LED ring
  ledRing.begin();
  ledRing.countdown();
}

void loop() {
  // Handle HTTP server requests
  webServer.handleClient();

  // Read button with debouncing
  static unsigned long lastDebounceTime = 0;
  static bool buttonState = false;
  static bool lastReading = HIGH;

  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > 50) {
    if (reading != buttonState) {
      buttonState = reading;
      bool buttonPressed = (buttonState == HIGH);
      if(buttonPressed) {
        speaker.soundWin();
        gameFinished = true;
      }
    }
  }

  lastReading = reading;

  // Control LED based on button
  bool buttonPressed = (buttonState == LOW);
  digitalWrite(CONTROL_LED_PIN, buttonPressed ? HIGH : LOW);

  unsigned long currentMillis = millis();

  // non-blocking LED blink
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }
}
