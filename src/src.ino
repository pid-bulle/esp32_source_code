const int LED_PIN = 15; // GPIO 15
bool ledState = LOW;
unsigned long previousMillis = 0;
const unsigned long interval = 500; // toggle every 500 ms -> 1 Hz blink

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 ready — blinking on GPIO15");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }
}
