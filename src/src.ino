#include <WiFi.h>
#include "WebServerController.h"

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

// Grove speaker on a PWM-capable pin (change as needed)
const int SPEAKER_PIN = 26; // wire Grove speaker signal to this pin
const int LEDC_CHANNEL = 0;

// Simple note frequency definitions (Hz)
#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494
#define NOTE_C5 523

// Example melody: "Twinkle Twinkle Little Star"
int melody[] = {
  NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4
};

int noteDurations[] = {
  4, 4, 4, 4, 4, 4, 2,
  4, 4, 4, 4, 4, 4, 2
};

unsigned long lastMelodyMillis = 0;
const unsigned long melodyInterval = 10000; // play melody every 10 seconds

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 ready — LED on GPIO15, speaker on GPIO26");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState);

  // Setup LEDC (ESP32 PWM) for tone generation
  ledcAttach(SPEAKER_PIN, 2000, 8); // pin, initial freq, resolution

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
}

void playMelody() {
  const int baseDuration = 500; // quarter note = 500 ms (adjust tempo)
  int notes = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < notes; i++) {
    int note = melody[i];
    int durationFactor = noteDurations[i];
    int noteLength = baseDuration / durationFactor;

    if (note == 0) {
      delay(noteLength);
    } else {
      ledcWriteTone(SPEAKER_PIN, note);
      delay(noteLength);
      ledcWriteTone(SPEAKER_PIN, 0); // stop tone
    }

    delay(50); // short pause between notes
  }
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

  // play melody periodically (this call blocks while melody plays)
  if (currentMillis - lastMelodyMillis >= melodyInterval) {
    lastMelodyMillis = currentMillis;
    playMelody();
  }
}
