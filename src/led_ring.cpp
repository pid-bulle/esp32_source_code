#include "led_ring.h"

LedRing::LedRing(uint8_t dataPin) : _pin(dataPin) {}

void LedRing::begin() {
  FastLED.addLeds<WS2812B, /*DATA_PIN*/ 0, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(180);
  FastLED.clear();
  FastLED.show();
}

// Helper: center LED solid, outer LEDs rotating fade
void LedRing::animateRing(CRGB color, uint16_t durationMs) {
  const uint16_t frameTime = 20;        // 50 FPS
  const uint16_t frames = durationMs / frameTime;

  for (uint16_t f = 0; f < frames; f++) {
    FastLED.clear();

    // Center LED steady
    leds[CENTER_LED] = color;

    // Rotating ring animation
    uint8_t pos = f % (NUM_LEDS - 1);

    for (uint8_t i = 1; i < NUM_LEDS; i++) {
      uint8_t distance = (i + (NUM_LEDS - 1) - pos) % (NUM_LEDS - 1);
      uint8_t brightness = max(0, 255 - distance * 60);

      leds[i] = color;
      leds[i].nscale8(brightness);
    }

    FastLED.show();
    delay(frameTime);
  }
}

void LedRing::showColor(CRGB c) {
  animateRing(c, 1000);
}

// Countdown sequence
void LedRing::countdown() {
  showColor(CRGB::Red);
  showColor(CRGB::Yellow);
  showColor(CRGB::Green);
}
